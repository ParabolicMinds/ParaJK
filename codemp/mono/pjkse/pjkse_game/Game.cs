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
	public static void CenterPrintClient(string message, Entity player) {
		GAME_INTERNAL_EXPORT.GMono_CenterPrint(message, player.clientNum, false?1:0);
	}
	public static void CenterPrintGlobal(string message) {
		GAME_INTERNAL_EXPORT.GMono_CenterPrint(message, -1, false?1:0);
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
	public static void AddShaderRemap(string oldShader, string newShader) {
		GAME_INTERNAL_EXPORT.GMono_RemapShader(oldShader, newShader);
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

	private class EventList<Value> : SortedList<int, List<Value>> {
		public void Add(int msec, Value value) {
			if (!this.ContainsKey(msec)) {
				base.Add(msec, new List<Value>() {value});
			} else {
				this[msec].Add(value);
			}
		}
		public void RemoveAll(Value v) {
			for (int i = this.Count - 1; i >= 0; i--) {
				List<Value> l = this.ElementAt(i).Value;
				l.RemoveAll(x => x.Equals(v));
				if (l.Count == 0)
					base.RemoveAt(i);
			}
		}
		public IEnumerable<KeyValuePair<int, Value>> GetEnumerable() {
			return this.SelectMany(x => x.Value.Select(y => new KeyValuePair<int, Value>(x.Key, y)));
		}
		public IEnumerable<Value> GetExpiredEvents(int curMsec, out IEnumerable<int> hitKeys) {
			hitKeys = this.Where(x => x.Key <= curMsec).Select(x => x.Key);
			return this.Where(x => x.Key <= curMsec).SelectMany(x => x.Value);
		}
		public void RemoveRange(IEnumerable<int> range) {
			foreach(int i in range) {
				this.Remove(i);
			}
		}
	}

	/*
	================================================================
	Simple Event (Event With No Arguments)
	================================================================
	*/
	EventList<Action> simpleEvents = new EventList<Action>();
	public void AddSimpleEvent(int future_msec, Action act) {
		int ftime = lastFrame + future_msec;
		simpleEvents.Add(ftime, act);
	}
	public void RemoveSimpleEvents(Action e) {
		simpleEvents.RemoveAll(e);
	}
	protected void RunSimpleEvents() {
		IEnumerable<int> hv;
		foreach(Action A in simpleEvents.GetExpiredEvents(lastFrame, out hv)) {
			try {
				A.Invoke();
			} catch (Exception e) {
				G.PrintLine(e.ToString());
			}
		}
		simpleEvents.RemoveRange(hv);
	}
	/*
	================================================================
	*/

	/*
	================================================================
	Complex Event (Event With Arguments)
	================================================================
	*/
	public struct ComplexGroup {public Object instance; public MethodInfo method; public Object[] parameters;}
	EventList<ComplexGroup> complexEvents = new EventList<ComplexGroup>();
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
	public void RemoveComplexEvents(ComplexGroup e) {
		complexEvents.RemoveAll(e);
	}
	protected void RunComplexEvents() {
		IEnumerable<int> hv;
		foreach(ComplexGroup cg in complexEvents.GetExpiredEvents(lastFrame, out hv)) {
			try {
				cg.method.Invoke(cg.instance, cg.parameters);
			} catch (Exception e) {
				G.PrintLine(e.ToString());
			}
		}
		complexEvents.RemoveRange(hv);
	}
	/*
	================================================================
	*/

	/*
	================================================================
	Simple Entity Event
	================================================================
	*/
	EventList<Tuple<Action, Entity>> simpleEntityEvents = new EventList<Tuple<Action, Entity>>();
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
		IEnumerable<int> hv;
		foreach(Tuple<Action, Entity> aec in simpleEntityEvents.GetExpiredEvents(lastFrame, out hv)) {
			try {
				aec.Item1.Invoke();
			} catch (Exception e) {
				G.PrintLine(e.ToString());
			}
		}
		simpleEntityEvents.RemoveRange(hv);
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