using System;

static class GAME_INTERNAL_IMPORT {
	static Entity curPlayer;

	public static void GMono_Init() {
		Entity testEnt = G.EntityRegistry.Register("Test Ent");
		testEnt.Model = "/models/items/a_pwr_converter.md3";
		testEnt.SetOrigin(0, 0, 0);
	}
	public static void GMono_Frame(int levelTime) {
		G.PrintLine("Frame Test");
		if (curPlayer != null)
			G.EntityRegistry["Test Ent"].SetOrigin(curPlayer.Origin);
	}
	public static void GMono_Shutdown() {
		G.EntityRegistry.Clear();
	}
	unsafe public static void Test(void * ent) {
		curPlayer = Entity.FromPtr((IntPtr)ent);
	}
}