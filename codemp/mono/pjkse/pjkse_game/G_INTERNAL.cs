using System;
using System.Runtime.CompilerServices;

public static class GAME_INTERNAL_EXPORT {

	//YOU, yes YOU, map maker. Try not to use these directly.

	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	unsafe public static extern void GMono_Kill ( IntPtr ent );

	[MethodImplAttribute(MethodImplOptions.InternalCall)]
	public static extern void GMono_Print ( string str );

}

