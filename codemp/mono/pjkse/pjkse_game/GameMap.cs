using System;
using System.Collections.Generic;
using System.CodeDom.Compiler;
using Microsoft.CSharp;
using System.Reflection;
using System.Linq;
using System.IO;

public static class MapCSBridge {
	private static List<string> cssources = new List<string>();

	private static List<MethodInfo> initEntries = new List<MethodInfo>();
	private static List<MethodInfo> frameEntries = new List<MethodInfo>();
	private static List<MethodInfo> shutdownEntries = new List<MethodInfo>();

	private static Dictionary<string, List<MethodInfo>> entityEntries = new Dictionary<string, List<MethodInfo>>();

	public static void BridgeInitialize(string name) {
		Load(name);
		CompileAndAssemble();
		foreach(MethodInfo m in initEntries)
			m.Invoke(null, new object[0]);
	}

	public static void BridgeFrame(int levelTime) {
		foreach(MethodInfo m in initEntries)
			m.Invoke(null, new object[] {levelTime});
	}

	public static void BridgeShutdown() {
		foreach(MethodInfo m in initEntries)
			m.Invoke(null, new object[0]);
		initEntries.Clear();
		frameEntries.Clear();
		shutdownEntries.Clear();
		entityEntries.Clear();
		cssources.Clear();
	}

	public static void BridgeEntity(Entity ent, string tag) {
		if (!entityEntries.ContainsKey(tag)) throw new Exception(String.Format("C# tag not found: \"{0}\"", tag));
		foreach(MethodInfo m in entityEntries[tag])
			m.Invoke(null, new object[] {ent});
	}

	private static void Load(string name) {
		cssources.Clear();
		try {
			GameFile pack;
			pack = new GameFile(String.Format("cs/{0}.cspack", name));
			if (!pack.isReady()) return;
			byte[] data = pack.Read();
			string contents = System.Text.Encoding.UTF8.GetString(data);
			pack.Close();

			string[] packentries = contents.Split(new char[] {'\r', '\n'}, StringSplitOptions.RemoveEmptyEntries);
			foreach (string entry in packentries) {
				try {
					GameFile csfile = new GameFile(String.Format("cs/{0}.cs", entry));
					byte[] csdata = csfile.Read();
					csfile.Close();
					string csstr = System.Text.Encoding.UTF8.GetString(csdata);
					cssources.Add(csstr);
				} catch (Exception e) {
					G.PrintLine(e.ToString());
					continue;
				}
			}
		} catch (Exception e) {
			G.PrintLine(e.ToString());
		}
		G.PrintLine("C# Scripts successfully loaded.");
	}
	private static void CompileAndAssemble() {
		if (cssources.Count < 1) {
			throw new Exception("No C# sources loaded, nothing to compile.");
		}
		CompilerResults results;
		CompilerParameters compParam = new CompilerParameters ();
		compParam.GenerateInMemory = true;
		compParam.GenerateExecutable = false;
		compParam.TreatWarningsAsErrors = false;
		compParam.CompilerOptions = "/optimize";
		compParam.ReferencedAssemblies.AddRange (new string[] {"System.dll", Path.Combine(Environment.CurrentDirectory, "para", "pjkse_game.dll")});
		CSharpCodeProvider provider = new CSharpCodeProvider ();
		results = provider.CompileAssemblyFromSource(compParam, cssources.ToArray());
		if (results.Output.Count > 0) {
			G.PrintLine("C# compile output:");
			foreach(string o in results.Output) {
				G.PrintLine(o);
			}
		}
		if (results.Errors.Count > 0) {
			G.PrintLine("C# compile errors:");
			foreach(CompilerError o in results.Errors) {
				G.PrintLine(o.ToString());
			}
			throw new Exception("C# Script compilation has errors... Cannot continue.");
		}

		G.PrintLine("C# Script compile completed. Probing for entry points...");

		Module mod = results.CompiledAssembly.GetModules()[0];
		IEnumerable<Type> pjkc = mod.GetTypes().Where(c => c.GetCustomAttributes(typeof(ParaJKEntryClass), true).Length > 0);
		if (pjkc.Count() <= 0) throw new Exception("C# Script has no entry points... Cannot continue.");

		foreach(Type t in pjkc) {
			foreach(MethodInfo m in t.GetMethods()) {
				foreach (object attr in m.GetCustomAttributes(true)) {
					if (attr.GetType() == typeof(ParaJKEntryInit)) {
						if (!m.IsStatic) throw new Exception(String.Format("FATAL ({0}): ParaJK Init Entry methods MUST be static.", m.Name));
						if (m.GetParameters().Length > 0) throw new Exception(String.Format("FATAL ({0}): ParaJK Init Entry methods MUST not have any arguments.", m.Name));
						if (m.ReturnParameter.GetType() != typeof(void)) G.PrintLine(String.Format("WARNING ({0}): ParaJK Init Entry has a return value, it will be ignored.", m.Name));
						initEntries.Add(m);
					}
					if (attr.GetType() == typeof(ParaJKEntryShutdown)) {
						if (!m.IsStatic) throw new Exception(String.Format("FATAL ({0}): ParaJK Shutdown Entry methods MUST be static.", m.Name));
						if (m.GetParameters().Length > 0) throw new Exception(String.Format("FATAL ({0}): ParaJK Shutdown Entry methods MUST not have any arguments.", m.Name));
						if (m.ReturnParameter.GetType() != typeof(void)) G.PrintLine(String.Format("WARNING ({0}): ParaJK Shutdown Entry has a return value, it will be ignored.", m.Name));
						shutdownEntries.Add(m);
					}
					if (attr.GetType() == typeof(ParaJKEntryFrame)) {
						if (!m.IsStatic) throw new Exception(String.Format("FATAL ({0}): ParaJK Frame Entry methods MUST be static.", m.Name));
						if (m.GetParameters().Length != 1) throw new Exception(String.Format("FATAL ({0}): ParaJK Frame Entry methods MUST have ONE argument. (Integer, Level Time in milliseconds)", m.Name));
						if (m.GetParameters()[0].ParameterType != typeof(int)) throw new Exception(String.Format("FATAL ({0}): ParaJK Frame Entry method's argument MUST be an integer.", m.Name));
						if (m.ReturnParameter.GetType() != typeof(void)) G.PrintLine(String.Format("WARNING ({0}): ParaJK Frame Entry has a return value, it will be ignored.", m.Name));
						frameEntries.Add(m);
					}
					if (attr.GetType() == typeof(ParaJKEntryEntity)) {
						if (!m.IsStatic) throw new Exception(String.Format("FATAL ({0}): ParaJK Entity Entry methods MUST be static.", m.Name));
						if (m.GetParameters().Length != 1) throw new Exception(String.Format("FATAL ({0}): ParaJK Entity Entry methods MUST have ONE argument. (Entity)", m.Name));
						if (m.GetParameters()[0].ParameterType != typeof(Entity)) throw new Exception(String.Format("FATAL ({0}): ParaJK Frame Entry method's argument MUST be an Entity.", m.Name));
						if (m.ReturnParameter.ParameterType.ReflectedType != typeof(void)) G.PrintLine(String.Format("WARNING ({0}): ParaJK Entity Entry has a return value, it will be ignored.", m.Name));
						foreach(string tag in ((ParaJKEntryEntity)attr).maptags) {
							G.PrintLine(tag);
							if (!entityEntries.ContainsKey(tag)) entityEntries[tag] = new List<MethodInfo>();
							entityEntries[tag].Add(m);
						}
					}
				}
			}
		}

		G.PrintLine("All C# script assembly operations completed successfully.");

	}
}

[AttributeUsage(AttributeTargets.Class)]
public class ParaJKEntryClass : Attribute {}
[AttributeUsage(AttributeTargets.Method)]
public class ParaJKEntryEntity : Attribute {
	public readonly string[] maptags;
	public ParaJKEntryEntity(params string[] tags) {maptags = tags;}
}
[AttributeUsage(AttributeTargets.Method)]
public class ParaJKEntryInit : Attribute {}
[AttributeUsage(AttributeTargets.Method)]
public class ParaJKEntryShutdown : Attribute {}
[AttributeUsage(AttributeTargets.Method)]
public class ParaJKEntryFrame : Attribute {}