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

### UE4SS mods

This mod requires UE4SS v3.0.1 experimental. This can be obtained from the release page here: https://github.com/UE4SS-RE/RE-UE4SS/releases/tag/experimental-latest

You only need the standard release, it should be named something similar to the image below

<img width="1197" height="301" alt="image" src="https://github.com/user-attachments/assets/c5967188-511a-4760-91cc-03dcd16cf366" />

UE4SS provides a dwmapi.dll and a ue4ss folder, these need to be extracted and placed next to the file KoFXV-Win64-Shipping.exe

An example for the directory is something like this: ...\steamapps\common\THE KING OF FIGHTERS XV\KOFXV\Binaries\Win64 - Do not place it next to the KOFXV_Steam.exe or similar found in the root THE KING OF FIGHTERS XV install folder.

Image example of how the folder should look

<img width="1990" height="154" alt="image" src="https://github.com/user-attachments/assets/1e3cab4b-d7b9-402c-9543-cb624c29519b" />

Once you have UE4SS in place, grab the latest version of the mod from: 

The release will contain a ue4ss folder and a ~mods folder.

You want to merge the ue4ss folder from the mod with the one you have installed earlier. Within the ue4ss folders there will be a Mods folder, and a mods.txt file alongside the mods. You want to overwrite the mods.txt file from ue4ss with the one contained within the mod.

<img width="950" height="396" alt="Screenshot 2026-04-11 144418" src="https://github.com/user-attachments/assets/5295612b-3854-45f2-8962-d651ecca224b" />

The Mods folder should now look like this.
Directory example: ..\steamapps\common\THE KING OF FIGHTERS XV\KOFXV\Binaries\Win64\ue4ss\Mods

The mods.txt file should look like this

<img width="436" height="380" alt="Screenshot 2026-04-11 144657" src="https://github.com/user-attachments/assets/015c435c-703a-4ac9-8361-d04b1003ae9f" />

You can enable other mods/install other mods using ue4ss by modifying this file, this is just set up to only use the mods included here. Mods can be independently disabled here by changing the 1 to a 0.

### .pak mods

The ~mods folder contains the .pak mods. These mods are direct changes to character files from the game and are not externally applied. These can be installed by placing the ~mods folder into the Paks directory. 
Example directory: ...\steamapps\common\THE KING OF FIGHTERS XV\KOFXV\Content\Paks

<img width="993" height="368" alt="Screenshot 2026-04-11 144943" src="https://github.com/user-attachments/assets/7ac5dae1-2c40-4d8f-bcf1-e20bf094caba" />

When installed the paks directory should look as such. These mods are not as easily uniquely enabled/disabled. To disable these mods you must delete the .pak and .sig files related to them.

## Testing

You can test in game by looking at character walk speeds, checking st.CD properties, checking if a reversal throw works from hard knockdown, checking if you receive the higher max meter, and checking the ability to stop from a run.
