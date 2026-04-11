# King of Fighters XV Mods

## Mod Details

### Throw Protect

This mod is to remove reversal normal throws from hard knockdowns and air resets in KoFXV, while retaining them on quick recovery making it more in line with other KoF games. 1 frame command throws are still valid reversals(like other KoF games).

### Run Lock

This mod is to adjust the forced running time when beginning a run. By default in game this is 14 frames, although the game considers 0 the first frame, so the in game calculation is 13 frames. In the mod file this is set to 4 which is 5 frames overall from a human perspective.

### MaxC3

This mod allocates the max mode timer to characters 1 and 2 to be the same as character 3. This unifies max mode across characters, increasing it's value in earlier rounds.

### .pak mods

The .pak mods found in the ~mods folder are as follows:

#### OriginalCDs.pak

Removes the wall splat st.CD attacks, replacing them with classic knockdown CD's that can be followed up on counter hit like air CD's in KoFXV. Maxima and Orochi Yashiro retain wall bounce properties on counter hit, similar to previous games unique CD wall bounces for some characters.

#### Movement_and_rush.pak

This mod decreases backwards roll velocity from -0.9 to -0.6 or equivelant. The only character with a different roll velocity is Shermie/Orochi Shermie, her roll is similarly adjusted. The intent is to make backwards rolls punishable by far normals/sweeps as they have been able to in previous games.
With the removal of wall splat CD's roll OS has become significantly weaker.

The mod also increases every characters walk speed linearly. Backwards and forwards walk speeds are still separate, and the difference between characters is maintained while improving walk speeds across the board to be more in line with previous games.

The mod also removes rush/auto combos from cl.A attacks.

The different changes of this mod cannot be independently separated although it is possible to create versions of the mod that would revert selected changes or allow for other additional character changes.

## Online Play

If you intend to play online with the mod both players must have the mods else you will desync. Playing matchmaking in ranked or elsewhere is not advised, consoles are also unable to use the mods.

## Install Instructions

This mod requires UE4SS v3.0.1 experimental. This can be obtained from the release page here: https://github.com/UE4SS-RE/RE-UE4SS/releases/tag/experimental-latest

You only need the standard release, it should be named something similar to the image below

<img width="1197" height="301" alt="image" src="https://github.com/user-attachments/assets/c5967188-511a-4760-91cc-03dcd16cf366" />

UE4SS provides a dwmapi.dll and a ue4ss folder, these need to be extracted and placed next to the file KoFXV-Win64-Shipping.exe

An example for the directory is something like this: ..\steamapps\common\THE KING OF FIGHTERS XV\KOFXV\Binaries\Win64 - Do not place it next to the KOFXV_Steam.exe or similar found in the root THE KING OF FIGHTERS XV install folder.

Image example of how the folder should look

<img width="1990" height="154" alt="image" src="https://github.com/user-attachments/assets/1e3cab4b-d7b9-402c-9543-cb624c29519b" />

Once you have UE4SS in place, grab the latest version of the mod from the Github Releases section ThrowProtect.zip: https://github.com/KM-Atma/ThrowProtect/releases

The release will contain a ThrowProtect folder and a mods.txt file.

In the ue4ss folder that you placed within the folder earlier there will be a Mods folder, extract the release inside of this mod folder and overwrite the existing mods.txt file.

Directory example: ..\steamapps\common\THE KING OF FIGHTERS XV\KOFXV\Binaries\Win64\ue4ss\Mods

<img width="1994" height="299" alt="image" src="https://github.com/user-attachments/assets/43d35e67-5bee-48b7-9da7-fa49b1669078" />

The Mods folder should now look like this

The mods.txt file should look like this

<img width="402" height="303" alt="image" src="https://github.com/user-attachments/assets/d2c3747e-b979-4083-9f18-a99d11a36fa4" />

You can enable other mods/install other mods using ue4ss by modifying this file, this is just set up to only use the ThrowProtect mod for ease of use.

You can check if it works in game by setting up a throw reversal in training mode and testing.
