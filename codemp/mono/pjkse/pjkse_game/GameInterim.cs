using System;
using System.Runtime.InteropServices;

public class Vec3 {
	public float X, Y, Z;
	public Vec3(float x, float y, float z) {
		X = x; Y = y; Z = z;
	}
	public Vec3(IntPtr unmanagedOrigin) {
		float[] temp = new float[3];
		Marshal.Copy(unmanagedOrigin, temp, 0, 3);
		X = temp[0]; Y = temp[1]; Z = temp[2];
	}
	public void Transpose(float xm, float ym, float zm) {
		X += xm; Y += ym; Z += zm;
	}
	public Vec3 Transposed(float xm, float ym, float zm) {
		return new Vec3(X + xm, Y + ym, Z + zm);
	}
}

public class Entity { 

	/*
		================================================================
		Private/Protected Fields
		================================================================
		*/
	protected IntPtr ent;
	/*
		================================================================
		*/

	/*
	================================================================
	Construction/Destruction
	================================================================
	*/
	private Entity(IntPtr ent) {
		this.ent = ent;
	}
	public static Entity FromPtr(IntPtr ptr) {
		return new Entity(ptr);
	}
	public IntPtr GetPtr() {return ent;}
	/*
		================================================================
		*/

	/*
	================================================================
	Public Information Helpers
	================================================================
	*/
	public bool IsNPC() {
		return GAME_INTERNAL_EXPORT.GMono_IsNPC (ent) != 0;
	}
	public bool IsPlayer() {
		return GAME_INTERNAL_EXPORT.GMono_IsPlayer (ent) != 0;
	}
	/*
		================================================================
		*/

	/*
	================================================================
	Public Properties
	================================================================
	*/
	public int Health {
		get {
			return GAME_INTERNAL_EXPORT.GMono_GetHealth(ent);
		}
		set {
			GAME_INTERNAL_EXPORT.GMono_SetHealth(ent, value);
		}
	}
	public Vec3 Origin {
		get {
			return new Vec3(GAME_INTERNAL_EXPORT.GMono_GetOriginPtr(ent));
		}
	}
	public string Model {
		get {
			return GAME_INTERNAL_EXPORT.GMono_GetModel(ent);
		}
		set {
			GAME_INTERNAL_EXPORT.GMono_SetModel(ent, value);
		}
	}
	/*
		================================================================
		*/

	/*
	================================================================
	Entity Interactions
	================================================================
	*/
	public void Kill() {
		if (!this.IsPlayer()) throw new Exception("Can only kill players, will add NPC support later.");
		GAME_INTERNAL_EXPORT.GMono_Kill (ent);
	}
	public void SetOrigin(float X, float Y, float Z) {
		if (this.IsPlayer()) throw new Exception("Cannot set origin on players for now, use Teleport()");
		GAME_INTERNAL_EXPORT.GMono_SetOrigin(ent, X, Y, Z);
	}
	public void SetOrigin(Vec3 newOrigin) {
		SetOrigin(newOrigin.X, newOrigin.Y, newOrigin.Z);
	}
	public void MoveTo(Vec3 location, int timespan_msec) {
		if (this.IsPlayer()) throw new Exception("Cannot move players for now, use Teleport()");
		GAME_INTERNAL_EXPORT.GMono_MoveTo(ent, location.X, location.Y, location.Z, (float)timespan_msec);
	}
	public void Move(Vec3 offset, int timespan_msec) {
		this.MoveTo(this.Origin.Transposed(offset.X, offset.Y, offset.Z), timespan_msec);
	}
	/*
		================================================================
		*/
}