using System;

namespace G {
	public class Entity {

		protected IntPtr ent;

		public Entity(IntPtr ent) {
			this.ent = ent;
		}

		public void Kill() {
			GAME_INTERNAL_EXPORT.GMono_Kill (ent);
		}
	}
}

