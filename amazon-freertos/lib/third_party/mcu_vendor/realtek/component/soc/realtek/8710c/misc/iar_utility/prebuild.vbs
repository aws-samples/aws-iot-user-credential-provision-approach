Option Explicit
DIM fso    

Dim WshShell

Set fso = CreateObject("Scripting.FileSystemObject")

If (fso.FileExists("""" & WScript.Arguments(0) & """\..\..\..\..\lib\third_party\mcu_vendor\realtek\component\soc\realtek\8710c\misc\iar_utility\prebuild.bat")) Then
  MsgBox "script not exist " & WScript.Arguments(0) & "\..\..\..\..\lib\third_party\mcu_vendor\realtek\component\soc\realtek\8710c\misc\iar_utility\prebuild.bat" , vbinformation
  WScript.Quit()
End If

Set WshShell = WScript.CreateObject("WScript.Shell")

WshShell.Run "cmd /c """"" & WScript.Arguments(0) & "\..\..\..\..\lib\third_party\mcu_vendor\realtek\component\soc\realtek\8710c\misc\iar_utility\prebuild.bat"" """ & WScript.Arguments(0) & """""", 0, true