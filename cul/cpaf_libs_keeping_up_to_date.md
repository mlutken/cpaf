# Updating github hosted libraries here from github using git subtrees

*IMPORTANT*: All commands are executed from the cpaf repo root!
If you need to do a similar import of a new subtree git project simply use 
*add* instead off *pull* for the initial import command. Like for example this:

(webcc example)
*cpaf$* git subtree add --prefix cul/webcc https://github.com/sprinfall/webcc.git master --squash

## webcc https://github.com/sprinfall/webcc.git
- *Update to latest:* *cpaf$* git subtree pull --prefix cul/webcc https://github.com/sprinfall/webcc.git master --squash

## luepp update
- *Update to latest:* *cpaf$* git subtree pull --prefix cul/luepp https://github.com/mlutken/luepp.git main --squash

## KZip update
- *Update to latest:* *cpaf$* git subtree pull --prefix cul/KZip https://github.com/troldal/KZip.git master --squash

## ImGuiFileDialog update
- *Update to latest:* *cpaf$* git subtree pull --prefix cul/ImGuiFileDialog https://github.com/aiekick/ImGuiFileDialog.git master --squash

## HowardHinnant date library update
- *Update to latest:* *cpaf$* git subtree pull --prefix cul/date https://github.com/HowardHinnant/date.git master --squash

## IconFontCppHeaders update
- *Update to latest:* *cpaf$* git subtree pull --prefix cul/IconFontCppHeaders/IconFontCppHeaders https://github.com/juliettef/IconFontCppHeaders.git main --squash

