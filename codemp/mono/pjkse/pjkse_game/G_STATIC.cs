using System;

static class GAME {

	unsafe static void KillEntity(void * ent) {
		GAME_IMPORT.GMono_Print ("TEST LOL\n");
		GAME_IMPORT.GMono_Kill (ent);
	}

}