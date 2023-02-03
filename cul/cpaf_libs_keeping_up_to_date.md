# Updating github hosted libraries here from github using git subtrees

*IMPORTANT*: All commands are executed from the crawler repo root!
If you need to do a similar import of a new subtree git project simply use 
*add* instead off *pull* for the initial import command. Like for example this:

(webcc example)
*crawler$* git subtree add --prefix cpaf/cul/webcc https://github.com/sprinfall/webcc.git master --squash

## webcc https://github.com/sprinfall/webcc.git
- *Update to latest:* *crawler$* git subtree pull --prefix cpaf/cul/webcc https://github.com/sprinfall/webcc.git master --squash


## xtensor update
- *Update to latest:* *crawler$* git subtree pull --prefix cpaf/cul/xtensor https://github.com/xtensor-stack/xtensor.git master --squash


## centurion update
- *Update to latest:* *crawler$* git subtree pull --prefix cpaf/cul/centurion https://github.com/albin-johansson/centurion.git main --squash

## luepp update
- *Update to latest:* *crawler$* git subtree pull --prefix cpaf/cul/luepp https://github.com/mlutken/luepp.git main --squash
