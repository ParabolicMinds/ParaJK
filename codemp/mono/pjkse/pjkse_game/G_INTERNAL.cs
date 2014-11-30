using System;
using System.Runtime.CompilerServices;

public static class GAME_IMPORT {

	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	unsafe public static extern void GMono_Kill ( void * ent );

	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern void GMono_Print ( string str );

}

