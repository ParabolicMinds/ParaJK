using System;

static class GAME_INTERNAL_IMPORT {

	unsafe static void Test(void * ent) {
		G.Entity ge = new G.Entity((IntPtr)ent);
		GAME_INTERNAL_EXPORT.GMono_Print ("TEST LOL\n");
		ge.Kill();
	}

}