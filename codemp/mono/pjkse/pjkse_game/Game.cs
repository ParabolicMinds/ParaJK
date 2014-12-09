using System;
using System.Collections.Generic; 

public static class G {
	public static void Print(string str) {
		GAME_INTERNAL_EXPORT.GMono_Print (str);
	}
	public static void PrintLine(string str) {
		GAME_INTERNAL_EXPORT.GMono_Print (str + '\n');
	}
	public static void Use(EntityPack ents, string target) {
		GAME_INTERNAL_EXPORT.GMono_UseTarget(ents.Self.GetPtr(), ents.Activator.GetPtr(), target);
	}
	public static EntityRegistry EntityRegistry = new EntityRegistry();
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