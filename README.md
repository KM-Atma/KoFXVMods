# King of Fighters XV Mods

## Mod Details

The mod is to remove reversal normal throws from hard knockdowns and air resets in KoFXV, while retaining them on quick recovery making it more in line with other KoF games. 1 frame command throws are still valid reversals(like other KoF games).

If you intend to play online with the mod - crossplay must be disabled and both players must have the mod else you will desync.

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
