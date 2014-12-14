using System;
using System.Runtime.InteropServices;
using System.Collections.Generic; 
using System.Linq;
using System.Reflection;

public static class G {
	public static void Print(string str) {
		GAME_INTERNAL_EXPORT.GMono_Print (str);
	}
	public static void PrintLine(string str) {
		GAME_INTERNAL_EXPORT.GMono_Print (str + '\n');
	}
	public static void CenterPrintClient(string message, Entity player, bool svlog = true) {
		GAME_INTERNAL_EXPORT.GMono_CenterPrint(message, player.clientNum, svlog?1:0);
	}
	public static void CenterPrintGlobal(string message, bool svlog = true) {
		GAME_INTERNAL_EXPORT.GMono_CenterPrint(message, -1, svlog?1:0);
	}
	public static void Use(EntityPack ents, string target) {
		GAME_INTERNAL_EXPORT.GMono_UseTarget(ents.Self.GetPtr(), ents.Activator.GetPtr(), target);
	}
	public static Entity[] FindEntities(string targetname, bool substring = false, int max = 16) {
		if (max < 0) throw new ArgumentOutOfRangeException("max cannot be less than 0.");
		IntPtr iarry = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(IntPtr)) * max);
		int got = GAME_INTERNAL_EXPORT.GMono_GetEntitiesByName(targetname, iarry, max, substring?1:0);
		Entity[] ents = new Entity[got];
		for (int i = 0; i < got; i++) {
			ents[i] = Entity.FromPtr((IntPtr)Marshal.PtrToStructure(IntPtr.Add(iarry, i * Marshal.SizeOf(typeof(IntPtr))), typeof(IntPtr)));
		}
		Marshal.FreeHGlobal(iarry);
		return ents;
	}
	public static EntityRegistry EntityRegistry = new EntityRegistry();
	public static FutureEvents FutureEvents = new FutureEvents();
}
	
public class EntityRegistry {
	protected Dictionary<string, Entity> reg = new Dictionary<string, Entity>();
	protected List<Entity> reg2 = new List<Entity>();

	internal EntityRegistry() {}

	public Entity Register(string key) {
		if (reg.ContainsKey(key)) {
			throw new Exception("Cannot register new entity, key already exists.");
		}
		Entity ent = Entity.FromPtr(GAME_INTERNAL_EXPORT.GMono_Spawn());
		ent.Valid = true;
		reg[key] = ent;
		return ent;
	}
	public Entity Register() {
		Entity ent = Entity.FromPtr(GAME_INTERNAL_EXPORT.GMono_Spawn());
		ent.Valid = true;
		reg2.Add(ent);
		return ent;
	}

	public bool ContainsKey(string key) {
		return reg.ContainsKey(key);
	}

	public Entity this[string key] {
		get {
			return reg[key];
		}
	}

	public void Free(string key) {
		if (!reg.ContainsKey(key)) throw new ArgumentException("An entity with this key was not found.");
		Entity ent = reg[key];
		reg.Remove(key);
		GAME_INTERNAL_EXPORT.GMono_Free(ent.GetPtr());
		ent = null;
	}

	public void Free(Entity ent) {
		for (int i = reg2.Count - 1; i >= 0; i--) {
			if (reg2.ElementAt(i).Equals(ent)) {
				GAME_INTERNAL_EXPORT.GMono_Free(ent.GetPtr());
				reg2.RemoveAt(i);
				return;
			}
		}
		throw new ArgumentException("Free was attempted on an unregistered entity. If the entity was registered with a key, it must be removed with a key.");
	}

	public void Clear() {
		foreach (Entity e in reg.Values) {
			GAME_INTERNAL_EXPORT.GMono_Free(e.GetPtr());
		}
		reg.Clear();
		foreach (Entity e in reg2) {
			GAME_INTERNAL_EXPORT.GMono_Free(e.GetPtr());
		}
		reg2.Clear();
	}
}

public class FutureEvents {
	private int lastFrame;

	/*
	================================================================
	Simple Event (Event With No Arguments)
	================================================================
	*/
	SortedList<int, Action> simpleEvents = new SortedList<int, Action>();
	public void AddSimpleEvent(int future_msec, Action act) {
		int ftime = lastFrame + future_msec;
		simpleEvents.Add(ftime, act);
	}
	public void RemoveSimpleEvents(Action e) {
		for (int i = simpleEvents.Count - 1; i >= 0; i--) {
			if (simpleEvents.ElementAt(i).Value.Equals(e)) simpleEvents.RemoveAt(i);
		}
	}
	protected void RunSimpleEvents() {
		for (int i = simpleEvents.Count - 1; i >= 0; i--) {
			KeyValuePair<int, Action> cur = simpleEvents.ElementAt(i);
			if (cur.Key <= lastFrame) {
				try {
					cur.Value.Invoke();
				} catch (Exception e) {
					G.PrintLine(e.ToString());
				} finally {
					simpleEvents.RemoveAt(i);
				}
			}
		}
	}
	/*
	================================================================
	*/

	/*
	================================================================
	Complex Event (Event With Arguments)
	================================================================
	*/
	private struct ComplexGroup {public Object instance; public MethodInfo method; public Object[] parameters;}
	SortedList<int, ComplexGroup> complexEvents = new SortedList<int, ComplexGroup>();
	public void AddComplexEvent(int future_msec, Type classType, string methodName, object instance, params object[] parameters) {
		int ftime = lastFrame + future_msec;
		MethodInfo meth = classType.GetMethod(methodName);
		if (meth == null) throw new ArgumentException(String.Format("Method {0} could not be found in class {1}.", methodName, classType.Name));
		if (!meth.IsStatic && instance == null) throw new ArgumentException(String.Format("Instance is null, but method {0} is not static.", meth.Name)); 
		if (meth.GetParameters().Length != parameters.Length) throw new ArgumentException(String.Format("Argument count mismatch: Method expects {0} args, {1} provided.", meth.GetParameters().Length, parameters.Length));
		ComplexGroup cg = new ComplexGroup();
		cg.instance = instance;
		cg.method = meth;
		cg.parameters = parameters;
		complexEvents.Add(ftime, cg);
	}
	protected void RunComplexEvents() {
		for (int i = complexEvents.Count - 1; i >= 0; i--) {
			KeyValuePair<int, ComplexGroup> cur = complexEvents.ElementAt(i);
			if (cur.Key <= lastFrame) {
				try {
					cur.Value.method.Invoke(cur.Value.instance, cur.Value.parameters);
				} catch (Exception e) {
					G.PrintLine(e.ToString());
				} finally {
					complexEvents.RemoveAt(i);
				}
			}
		}
	}
	/*
	================================================================
	*/

	/*
	================================================================
	Simple Entity Event
	================================================================
	*/
	SortedList<int, Tuple<Action, Entity>> simpleEntityEvents = new SortedList<int, Tuple<Action, Entity>>();
	public void AddSimpleEntityEvent(int future_msec, Entity ent, Action act) {
		int ftime = lastFrame + future_msec;
		simpleEntityEvents.Add(ftime, new Tuple<Action, Entity>(act, ent));
	}
	public void RemoveSimpleEntityEvents(Entity ent, Action act) {
		for (int i = simpleEntityEvents.Count - 1; i >= 0; i--) {
			if (simpleEntityEvents.ElementAt(i).Value.Equals(new Tuple<Action, Entity>(act, ent))) simpleEntityEvents.RemoveAt(i);
		}
	}
	protected void RunSimpleEntityEvents() {
		for (int i = simpleEntityEvents.Count - 1; i >= 0; i--) {
			KeyValuePair<int, Tuple<Action, Entity>> cur = simpleEntityEvents.ElementAt(i);
			if (cur.Key <= lastFrame) {
				try {
					cur.Value.Item1.Invoke();
				} catch (Exception e) {
					G.PrintLine(e.ToString());
				} finally {
					simpleEntityEvents.RemoveAt(i);
				}
			}
		}
	}
	/*
	================================================================
	*/
	/*
	================================================================
	Simple Entity Event
	================================================================
	*/
	SortedList<int, Entity> freeEvents = new SortedList<int, Entity>();
	public void AddFreeEvent(int future_msec, Entity ent) {
		int ftime = lastFrame + future_msec;
		freeEvents.Add(ftime, ent);
	}
	public void RemoveFreeEvents(Entity ent) {
		for (int i = freeEvents.Count - 1; i >= 0; i--) {
			if (freeEvents.ElementAt(i).Value.Equals(ent)) freeEvents.RemoveAt(i);
		}
	}
	protected void RunFreeEvents() {
		for (int i = freeEvents.Count - 1; i >= 0; i--) {
			KeyValuePair<int, Entity> cur = freeEvents.ElementAt(i);
			if (cur.Key <= lastFrame) {
				try {
					G.EntityRegistry.Free(cur.Value);
				} catch (Exception e) {
					G.PrintLine(e.ToString());
				} finally {
					freeEvents.RemoveAt(i);
				}
			}
		}
	}
	/*
	================================================================
	*/

	internal void RunFrame(int leveltime) {
		lastFrame = leveltime;
		RunSimpleEvents();
		RunSimpleEntityEvents();
		RunComplexEvents();
		RunFreeEvents();
	}
	internal FutureEvents() {}
}