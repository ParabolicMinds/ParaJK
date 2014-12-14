using System;
using System.Runtime.CompilerServices;

internal static class GAME_INTERNAL_EXPORT {

	//ENTITY MANAGEMENT
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern IntPtr GMono_Spawn ();
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern void GMono_Free ( IntPtr ent );

	//Entity
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
	public static extern int GMono_GetSkinIndex ( IntPtr ent );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern void GMono_SetSkinIndex ( IntPtr ent, int newIndex );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern IntPtr GMono_GetModelScale ( IntPtr ent );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern void GMono_SetModelScale ( IntPtr ent, float X, float Y, float Z );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern IntPtr GMono_GetOriginPtr ( IntPtr ent );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern void GMono_SetOrigin ( IntPtr ent, float X, float Y, float Z );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern void GMono_MoveTo ( IntPtr ent, float X, float Y, float Z, float timespan );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern void GMono_MoveStop ( IntPtr ent );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern string GMono_GetModel ( IntPtr ent );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern void GMono_SetModel ( IntPtr ent, string newName );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern string GMono_GetTargetname ( IntPtr ent );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern void GMono_SetTargetname ( IntPtr ent, string newName );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern void GMono_UseTarget ( IntPtr self, IntPtr activator, string target );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern void GMono_UseEntity ( IntPtr self, IntPtr other, IntPtr activator );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern int GMono_GetEntityType ( IntPtr ent );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern void GMono_SetEntityType ( IntPtr ent, int t );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern int GMono_GetClientNum ( IntPtr ent );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern int GMono_GetEntitiesByName(string targetname, IntPtr buffer, int bufptrlen, int substr);
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern IntPtr GMono_GetHitboxMin ( IntPtr ent );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern IntPtr GMono_GetHitboxMax ( IntPtr ent );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern void GMono_SetHitbox ( IntPtr ent, float Xmin, float Ymin, float Zmin, float Xmax, float Ymax, float Zmax );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern int GMono_GetSolid ( IntPtr ent );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern void GMono_SetSolid ( IntPtr ent, int solid );

	//SERVER
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern void GMono_Print ( string str );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern void GMono_CenterPrint ( string msg, int clinum, int svprnt );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern int GMono_FS_Open ( string path, IntPtr empty_int32_handle );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern int GMono_FS_Read ( IntPtr buffer, int buflen, Int32 handle );
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern void GMono_FS_Close ( Int32 handle );

}

