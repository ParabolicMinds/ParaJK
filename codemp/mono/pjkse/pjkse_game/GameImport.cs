using System;

static class GAME_INTERNAL_IMPORT {

	public static void GMono_Initialize(string name) {
		try {
			MapCSBridge.BridgeInitialize(name);
		} catch (Exception e) {
			Console.WriteLine("Exception occured during loading or compilation:");
			G.PrintLine(e.ToString());
		}
	}

	public static void GMono_Frame(int levelTime) {
		MapCSBridge.BridgeFrame(levelTime);
	}

	public static void GMono_Shutdown() {
		MapCSBridge.BridgeShutdown();
		G.EntityRegistry.Clear();
	}

	unsafe public static void GMono_EntityEntry(void * entPtr, string tag) {
		Entity ent = Entity.FromPtr(new IntPtr(entPtr));
		MapCSBridge.BridgeEntity(ent, tag);
	}
}