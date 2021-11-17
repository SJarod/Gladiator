# Gladiator
###### Loan Campan & Jarod Sengkeo

## Presentation
This project is a 3D third person view game made with Unreal Engine 4.
The game takes place in an arena where the player plays a gladiator that fights other gladiators.

## Controls
| Key | description |
| -- | -- |
| W, A, S, D | move the player |
| Space | jump |
| LMB | attack |
| RMB | block |
| Mouse Wheel ↑ | zoom in |
| Mouse Wheel ↓ | zoom out |

## Features
- C++
    - player
        - attack, block
        - damage material color
    - enemies
        - AI Controller
        - behavior tree's tasks, decorators and services
        - attack
        - enemy manager controlling every enemy
- Bluprints
    - blended character animations
    - enemies
        - AI using behavior tree
    - UI
        - life bars
        - main menu
        - "win" screen
        - "game over" screen

## Bugs/Problems
- attacks sometimes go through shield
- enemies are not easy to kill when they walk backwards
- enemies sometimes hide behind each other (line of sight not so clear)

## Next features
- add sounds
- pause menu