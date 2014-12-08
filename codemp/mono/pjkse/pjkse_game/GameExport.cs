using System;
using System.Runtime.CompilerServices;

public static class GAME_INTERNAL_EXPORT {

	//YOU, yes YOU, map maker. Try not to use these directly.


	//ENTITY MANAGEMENT
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern IntPtr GMono_Spawn ();
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern void GMono_Free ( IntPtr ent );

	//NPC/PLAYER
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern void GMono_Kill ( IntPtr ent );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern int GMono_IsPlayer ( IntPtr ent );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern int GMono_IsNPC ( IntPtr ent );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern int GMono_GetHealth ( IntPtr ent );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern void GMono_SetHealth ( IntPtr ent, int newHealth );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern IntPtr GMono_GetOriginPtr ( IntPtr ent );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern void GMono_SetOrigin ( IntPtr ent, float X, float Y, float Z );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern void GMono_MoveTo ( IntPtr ent, float X, float Y, float Z, float timespan );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern string GMono_GetModel ( IntPtr ent );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern void GMono_SetModel ( IntPtr ent, string newName );

	//SERVER
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern void GMono_Print ( string str );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern int GMono_FS_Open ( string path, IntPtr empty_int32_handle );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern int GMono_FS_Read ( IntPtr buffer, int buflen, Int32 handle );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern void GMono_FS_Close ( Int32 handle );

}

