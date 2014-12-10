using System;
using System.Runtime.InteropServices;

internal static class GAME_INTERNAL_IMPORT {

	public static void GMono_Initialize(string name) {
		G.PrintLine("\n===== C# SCRIPTS BEGIN =====");
		try {
			MapCSBridge.BridgeInitialize(name);
		} catch (Exception e) {
			Console.WriteLine("Exception occured during loading or compilation:");
			G.PrintLine(e.ToString());
		}
		G.PrintLine("=====  C# SCRIPTS END  =====\n");
	}

	public static void GMono_Frame(int levelTime) {
		G.FutureEvents.RunFrame(levelTime);
		MapCSBridge.BridgeFrame(levelTime);
	}

	public static void GMono_Shutdown() {
		MapCSBridge.BridgeShutdown();
		G.EntityRegistry.Clear();
	}

	public static void GMono_Reset() {
		G.CenterPrintGlobal("C# Scripts Reloading...\nExpect massive lag on complex maps or slow servers.", false);
		G.FutureEvents.AddSimpleEvent(1000, MapCSBridge.Reload);
	}

	unsafe public static void GMono_EntityEntry(string tag, void * self, void * activator, int count1, void * entptr1, int count2, void * entptr2, int count3, void * entptr3, int count4, void * entptr4) {
		Entity Self = Entity.FromPtr(new IntPtr(self));
		Entity Activator = Entity.FromPtr(new IntPtr(activator));
		Entity[] TargetsOne = new Entity[count1];
		if (TargetsOne.Length > 0) {
			IntPtr t1start = new IntPtr(entptr1);
			IntPtr[] ents1 = new IntPtr[count1];
			Marshal.Copy(t1start, ents1, 0, count1);
			for (int i = 0; i < count1; i++) {
				TargetsOne[i] = Entity.FromPtr(ents1[i]);
			}
		}
		Entity[] TargetsTwo = new Entity[count2];
		if (TargetsTwo.Length > 0) {
			IntPtr t2start = new IntPtr(entptr2);
			IntPtr[] ents2 = new IntPtr[count2];
			Marshal.Copy(t2start, ents2, 0, count2);
			for (int i = 0; i < count2; i++) {
				TargetsTwo[i] = Entity.FromPtr(ents2[i]);
			}
		}
		Entity[] TargetsThree = new Entity[count3];
		if (TargetsThree.Length > 0) {
			IntPtr t3start = new IntPtr(entptr3);
			IntPtr[] ents3 = new IntPtr[count3];
			Marshal.Copy(t3start, ents3, 0, count3);
			for (int i = 0; i < count3; i++) {
				TargetsThree[i] = Entity.FromPtr(ents3[i]);
			}
		}
		Entity[] TargetsFour = new Entity[count4];
		if (TargetsFour.Length > 0) {
			IntPtr t4start = new IntPtr(entptr4);
			IntPtr[] ents4 = new IntPtr[count4];
			Marshal.Copy(t4start, ents4, 0, count4);
			for (int i = 0; i < count4; i++) {
				TargetsFour[i] = Entity.FromPtr(ents4[i]);
			}
		}
		EntityPack ep = new EntityPack (Self, Activator, TargetsOne, TargetsTwo, TargetsThree, TargetsFour);
		MapCSBridge.BridgeEntity(ep, tag);
	}
}