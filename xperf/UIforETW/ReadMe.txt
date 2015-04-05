This is the to-do list for UIforETW, a UI to wrap recording ETW traces with xperf.exe.

Done:
Get appropriate default for tracedir.
Create tracedir as needed.
Launch wpa after a trace finishes recording.
Disable start/stop/snap buttons as appropriate.
Show commands and command output.
Add verbose option (default is off) to control whether commands are displayed.
Print readable, well-spaced information about the state of tracing.
Respect tracedir and temptracedir.
Generate good trace names, with built-in keywords.
Input recording.
Move more initialization like _NT_SYMBOL_PATH to startup.
Process Chrome symbols.
Finish input recording options and give warning about full input recording.
List all traces in tracedir and allow viewing them.
Register providers.
set DisablePagingExecutive to 1
Vertically resizable window to show more traces.
Editing and auto-saving of trace notes.
Stop tracing on shutdown!!!
Hot key to stop/record traces.
Make sure trace notes are saved when closing.
Tool-tips
Add circular-buffer support.
Heap tracing.
Show ProcessChromeSymbols.py status as it runs so it doesn't look like it's hung forever.
Rename output executables and check in a renamed copy so people don't have to build it.
StripChromeSymbols.py needs to pass the appropriate command-line to xperf to ignored dropped events and time inversions.
Unicode support
Trace list should let the user:
- Delete traces
- Compress traces
- Explore to the trace directory
- Copy the traces name/path to the clipboard
- Run StripChromeSymbols.py
Remove references to _T macro.
Remove .etl extension from list of traces -- it adds no value and wastes space.
Make sure trace notes are disabled when no trace is selected, including when
traces are added or deleted.
Move focus away from buttons before they are disabled, as when starting a trace with Alt+T.
Error checking -- checking for failures to start or stop tracing.
Renaming of traces and associated files.
Need keyboard accelerators for F2 (start renaming), enter (stop renaming),
and probably ESC (to stop it from closing the dialog).
Add DX provider.
Copy over startup profile on first-run, and subsequent runs?
Optionally copy over 64-bit dbghelp.dll and symsrv.dll?
Remember all settings.
Support for 32-bit operating systems.
Add compatibility manifest up to Windows 8.1
Add OS specific checks for what user providers to enable, compression options, etc.
Increase the user-provider buffer counts when doing DX profiling.
Disable compress options (checkbox and menu) for Windows 7 and below.
- Move GetPython function to utility.cpp
Don't allow having two copies of UIforETW to run simultaneously.
Ignore Ctrl+Win+C when tracing is halted.
Need more keyboard accelerators, to delete traces, view them, copy names to the clipboard, select-all in the notes field, etc.
Detect and handle trace files that lack the date component - still allow renaming.



Most important tasks:
Only have the delete key active when the trace list is active - otherwise it interferes with renaming.
Fix StripChromeSymbols.py so that it can find RetrieveSymbols.exe - copy Microsoft DLLs over?
Figure out why .txt files keep getting left behind - race condition in renaming or deleting or???
Create an installer that will install the MFC DLLs: https://msdn.microsoft.com/en-us/library/dd293568.aspx
Implement more settings - configure trace directories, buffer sizes,
option for stacks on user events.
Move ETWEventDemo out of bin directory.

To-do eventually:
Should have a Chrome developer checkbox.
Should have the option to run arbitrary scripts after each trace is recorded.
PreprocessTrace should append to the trace text file.
Should have an option to put an entire process tree in the trace text file.
ChildProcess or PreprocessTrace should convert from LF to CRLF for the edit control.
Try using the -cancel option for more efficient stopping of traces without recording. See xperf -help stop
Place the trace-name editing box appropriately.
Give the user a chance to rename trace before launching viewer
Transparent compression/decompression into .zip files. Note that if a trace is
compressed and then renamed that the trace inside the .zip file will be wrong - it
should be fixed up when the renaming happens.
Have a button to copy the recommended (or one of several recommended) startup profiles. Perhaps
a first-run configuration dialog?
Resize output window as well when sizing the window, just a bit.
Code cleanup:
- getenv wrapper
- ordering code sanely
- moving more code to separate functions/files
- Correct usage of bool as function parameters to choose behaviors

Unimportant:
Allow configuring which symbols should be stripped.
Configure a maximum time to trace for to avoid infinitely long traces.
CPU frequency monitoring.
Trace list should let the user:
- Run arbitrary scripts on all traces
