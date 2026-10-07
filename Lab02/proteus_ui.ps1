param([int]$X=-1,[int]$Y=-1,[string]$Keys='', [switch]$DoubleClick)
Add-Type -AssemblyName UIAutomationClient
Add-Type -AssemblyName System.Drawing
Add-Type -AssemblyName System.Windows.Forms
Add-Type @'
using System;
using System.Runtime.InteropServices;
public static class ProteusInput {
 [DllImport("user32.dll")] public static extern bool SetForegroundWindow(IntPtr h);
 [DllImport("user32.dll")] public static extern bool SetCursorPos(int x,int y);
 [DllImport("user32.dll")] public static extern void mouse_event(uint flags,uint x,uint y,uint data,UIntPtr extra);
}
'@
$root=[System.Windows.Automation.AutomationElement]::RootElement
$condition=[System.Windows.Automation.PropertyCondition]::new([System.Windows.Automation.AutomationElement]::NameProperty,'New Project - Proteus 8 Professional - Schematic Capture')
$window=$root.FindFirst([System.Windows.Automation.TreeScope]::Children,$condition)
if (!$window) { throw 'Proteus schematic window not found' }
$bounds=$window.Current.BoundingRectangle
[ProteusInput]::SetForegroundWindow([IntPtr]$window.Current.NativeWindowHandle) | Out-Null
if ($X -ge 0 -and $Y -ge 0) {
 [ProteusInput]::SetCursorPos([int]$bounds.X+$X,[int]$bounds.Y+$Y) | Out-Null
 [ProteusInput]::mouse_event(2,0,0,0,[UIntPtr]::Zero)
 [ProteusInput]::mouse_event(4,0,0,0,[UIntPtr]::Zero)
 if ($DoubleClick) {
  [ProteusInput]::mouse_event(2,0,0,0,[UIntPtr]::Zero)
  [ProteusInput]::mouse_event(4,0,0,0,[UIntPtr]::Zero)
 }
}
if ($Keys) { [System.Windows.Forms.SendKeys]::SendWait($Keys) }
Start-Sleep -Milliseconds 300
$bitmap=[System.Drawing.Bitmap]::new([int]$bounds.Width,[int]$bounds.Height)
$graphics=[System.Drawing.Graphics]::FromImage($bitmap)
$graphics.CopyFromScreen([int]$bounds.X,[int]$bounds.Y,0,0,$bitmap.Size)
$bitmap.Save((Join-Path $PSScriptRoot 'proteus-inspect.png'))
$graphics.Dispose()
$bitmap.Dispose()
