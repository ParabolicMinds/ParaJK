using System;
using System.Collections.Generic; 
using System.Linq;

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
	public static EntityRegistry EntityRegistry = new EntityRegistry();
	public static FutureEvents FutureEvents = new FutureEvents();
}
	
public class EntityRegistry {
	protected Dictionary<string, Entity> reg = new Dictionary<string, Entity>();

	internal EntityRegistry() {}

	public Entity Register(string key) {
		if (reg.ContainsKey(key)) {
			throw new Exception("Cannot register new entity, key already exists.");
		}
		Entity ent = Entity.FromPtr(GAME_INTERNAL_EXPORT.GMono_Spawn());
		reg[key] = ent;
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
		if (!reg.ContainsKey(key)) return;
		Entity ent = reg[key];
		reg.Remove(key);
		GAME_INTERNAL_EXPORT.GMono_Free(ent.GetPtr());
		ent = null;
	}

	public void Clear() {
		foreach (Entity e in reg.Values) {
			GAME_INTERNAL_EXPORT.GMono_Free(e.GetPtr());
		}
		reg.Clear();
	}
}

public class FutureEvents {
	private int lastFrame;
	List<int> rmTimes = new List<int>();

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
	SortedList<int, Tuple<Action, object[]>> complexEvents;
	public void AddComplexEvent(int future_msec, Action act, params object[] parameters) {
		int ftime = lastFrame + future_msec;
		complexEvents.Add(ftime, new Tuple<Action, object[]>(act, parameters));
	}
	public void RemoveComplexEvents(Action e) {
		foreach(KeyValuePair<int, Tuple<Action, object[]>> KVP in complexEvents) {

		}
	}
	protected void RunComplexEvents() {
		rmTimes.ForEach(x => complexEvents.Remove(x));
	}
	/*
	================================================================
	*/

	/*
	================================================================
	Simple Entity Event
	================================================================
	*/

	SortedDictionary<int, List<Tuple<Entity, Action>>> simpleEntityEvents = new SortedDictionary<int, List<Tuple<Entity, Action>>>();
	public void AddSimpleEntityEvent(int future_msec, Entity e, Action act) {
		int ftime = lastFrame + future_msec;
		if (!simpleEntityEvents.ContainsKey(ftime)) simpleEntityEvents[ftime] = new List<Tuple<Entity, Action>>();
		simpleEntityEvents[ftime].Add(new Tuple<Entity, Action>(e, act));
	}
	public void RemoveSimpleEntityEvents(Entity e, Action act) {
		foreach (KeyValuePair<int, List<Tuple<Entity, Action>>> KVP in simpleEntityEvents) {
			KVP.Value.RemoveAll(x => (x.Item1.Equals(e)) && (x.Item2.Method.Name == act.Method.Name));
		}
	}
	protected void RunSimpleEntityEvents() {
		foreach (KeyValuePair<int, List<Tuple<Entity, Action>>> KVP in simpleEntityEvents) {
			if (lastFrame > KVP.Key) {
				KVP.Value.ForEach(x => x.Item2.Invoke());
				rmTimes.Add(KVP.Key);
			}
		}
		rmTimes.ForEach(x => simpleEntityEvents.Remove(x));
	}
	/*
	================================================================
	*/

	internal void RunFrame(int leveltime) {
		lastFrame = leveltime;
		RunSimpleEvents();
		rmTimes.Clear();
		RunSimpleEntityEvents();
		rmTimes.Clear();
	}
	internal FutureEvents() {}
}