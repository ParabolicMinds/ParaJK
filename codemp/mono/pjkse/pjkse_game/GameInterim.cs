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
	public void Normalize() {
		float max = this.X;
		max = Math.Max(max, this.Y);
		max = Math.Max(max, this.Z);
		this.X /= max;
		this.Y /= max;
		this.Z /= max;
	}
	public Vec3 Normalized() {
		Vec3 nv = new Vec3(X, Y, Z);
		nv.Normalize();
		return nv;
	}
	public void Scale(float x, float y, float z) {
		X *= x; Y *= y; Z *= z;
	}
	public Vec3 Scaled(float x, float y, float z) {
		Vec3 nv = new Vec3(X, Y, Z);
		nv.Scale(x, y, z);
		return nv;
	}
	public static float Distance(Vec3 A, Vec3 B) {
		return (float)Math.Abs(Math.Sqrt(Math.Pow(A.X - B.X, 2)+Math.Pow(A.Y - B.Y, 2)+Math.Pow(A.Z - B.Z, 2)));
	}
	public float Length() {
		return (float)Math.Abs(Math.Sqrt(Math.Pow(X, 2)+Math.Pow(Y, 2)+Math.Pow(Z, 2)));
	}
	public static Vec3 operator-(Vec3 A, Vec3 B) {
		return new Vec3(A.X - B.X, A.Y - B.Y, A.Z - B.Z);
	}
	public static Vec3 operator+(Vec3 A, Vec3 B) {
		return new Vec3(A.X + B.X, A.Y + B.Y, A.Z + B.Z);
	}
	public static Vec3 operator*(Vec3 A, float B) {
		return new Vec3(A.X * B, A.Y * B, A.Z * B);
	}
	public static Vec3 operator/(Vec3 A, float B) {
		return new Vec3(A.X / B, A.Y / B, A.Z / B);
	}
	public override string ToString () {
		return string.Format ("({0}, {1}, {2})", X, Y, Z);
	}
}

public class Box {
	public Vec3 Min;
	public Vec3 Max;
	public Box(Vec3 Min, Vec3 Max) {
		this.Min = Min;
		this.Max = Max;
	}
	public Vec3 Difference() {
		return Max - Min;
	}
	public float Height {
		get {
			return Max.Y - Min.Y;
		}
	}
	public float WidthX {
		get {
			return Max.X - Min.X;
		}
	}
	public float WidthZ {
		get {
			return Max.Z - Min.Z;
		}
	}
}

public class GameFile {
	Int32 handle;
	public GameFile(string path) {
		IntPtr newHandle = Marshal.AllocHGlobal(4);
		bool exists = (GAME_INTERNAL_EXPORT.GMono_FS_Open(path, newHandle) != -1);
		if (!exists) {
			throw new Exception(String.Format("Error: failed to find and/or open: \"{0}\"", path));
		}
		handle = Marshal.ReadInt32(newHandle);
		Marshal.FreeHGlobal(newHandle);
	}
	public byte[] Read() {
		int buflen = 65535;
		IntPtr buffer = Marshal.AllocHGlobal(buflen);
		int len = GAME_INTERNAL_EXPORT.GMono_FS_Read(buffer, buflen, handle);
		if (len < 0) {
			Marshal.FreeHGlobal(buffer);
			throw new Exception("FATAL: File read returned <0 length.");
		}
		byte[] rbuf = new byte[len];
		Marshal.Copy(buffer, rbuf, 0, len);
		Marshal.FreeHGlobal(buffer);
		return rbuf;
	}
	public void Close() {
		GAME_INTERNAL_EXPORT.GMono_FS_Close(handle);
	}
	public bool isReady() {
		return handle != 0;
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
	public Entity () {}
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
	public int SkinIndex {
		get {
			return GAME_INTERNAL_EXPORT.GMono_GetSkinIndex(ent);
		}
		set {
			GAME_INTERNAL_EXPORT.GMono_SetSkinIndex(ent, value);
		}
	}
	public Vec3 Scale {
		get {
			return new Vec3(GAME_INTERNAL_EXPORT.GMono_GetModelScale(ent));
		}
		set {
			GAME_INTERNAL_EXPORT.GMono_SetModelScale(ent, value.X, value.Y, value.Z);
		}
	}
	public Vec3 Origin {
		get {
			return new Vec3(GAME_INTERNAL_EXPORT.GMono_GetOriginPtr(ent));
		}
	}
	public string Model {
		get {
			string m = GAME_INTERNAL_EXPORT.GMono_GetModel(ent);
			return m == null ? String.Empty : m;
		}
		set {
			GAME_INTERNAL_EXPORT.GMono_SetModel(ent, value);
		}
	}
	public string Targetname {
		get {
			string m = GAME_INTERNAL_EXPORT.GMono_GetTargetname(ent);
			return m == null ? String.Empty : m;
		}
		set {
			GAME_INTERNAL_EXPORT.GMono_SetTargetname(ent, value);
		}
	}
	public EntityType Type {
		get {
			return (EntityType)GAME_INTERNAL_EXPORT.GMono_GetEntityType(ent);
		}
		set {
			GAME_INTERNAL_EXPORT.GMono_SetEntityType(ent, (int)value);
		}
	}
	public Box Hitbox {
		get {
			return new Box(new Vec3(GAME_INTERNAL_EXPORT.GMono_GetHitboxMin(ent)), new Vec3(GAME_INTERNAL_EXPORT.GMono_GetHitboxMax(ent)));
		}
		set {
			GAME_INTERNAL_EXPORT.GMono_SetHitbox(ent, value.Min.X, value.Min.Y, value.Min.Z, value.Max.X, value.Max.Y, value.Max.Z);
		}
	}
	public bool Solid {
		get {
			return GAME_INTERNAL_EXPORT.GMono_GetSolid(ent) != 0;
		}
		set {
			GAME_INTERNAL_EXPORT.GMono_SetSolid(ent, value?1:0);
		}
	}
	public int clientNum {
		get {
			if (!IsPlayer()) throw new Exception("Attempted to retreive the client number of a non-player entity.");
			return GAME_INTERNAL_EXPORT.GMono_GetClientNum(ent);
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
	public void Use(EntityPack user) {
		GAME_INTERNAL_EXPORT.GMono_UseEntity(user.Self.GetPtr(), this.ent, user.Activator.GetPtr());
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
		G.FutureEvents.RemoveSimpleEntityEvents(this, this.MoveStop);
		GAME_INTERNAL_EXPORT.GMono_MoveTo(ent, location.X, location.Y, location.Z, (float)timespan_msec);
		G.FutureEvents.AddSimpleEntityEvent(timespan_msec, this, this.MoveStop);
	}
	public void MoveStop() {
		GAME_INTERNAL_EXPORT.GMono_MoveStop(ent);
	}
	public void Move(Vec3 offset, int timespan_msec) {
		this.MoveTo(this.Origin.Transposed(offset.X, offset.Y, offset.Z), timespan_msec);
	}
	/*
	================================================================
	*/

	/*
	================================================================
	Operation
	================================================================
	*/
	public override bool Equals (object obj) {
		Entity ent2 = (Entity)obj;
		if (ent2 == null) return false;
		return ent == ent2.ent;
	}
	public override int GetHashCode () {
		return ent.ToInt32();
	}
	public override string ToString () {
		return string.Format ("[Entity: Pointer={0}, Targetname=\"{3}\", Origin={1}, Type={2}]", ent.ToString(), Origin, Type, Targetname);
	}
	/*
	================================================================
	*/
}

public enum EntityType {
	General,
	Player,
	Item,
	Missile,
	Special,
	Holocron,
	Mover,
	Beam,
	Portal,
	Speaker,
	PushTrigger,
	TeleportTrigger,
	Invisible,
	NPC,
	Team,
	Body,
	Terrain,
	FX,
	Event
}

public class EntityPack {
	public Entity Self;
	public Entity Activator;
	public Entity[] TargetsOne;
	public Entity[] TargetsTwo;
	public Entity[] TargetsThree;
	public Entity[] TargetsFour;
	public EntityPack(Entity Self, Entity Activator, Entity[] T1, Entity[] T2, Entity[] T3, Entity[] T4) {
		this.Self = Self;
		this.Activator = Activator;
		this.TargetsOne = T1;
		this.TargetsTwo = T2;
		this.TargetsThree = T3;
		this.TargetsFour = T4;
	}
}