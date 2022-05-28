StepMania 5.1 to ITGmania Migration Guide
=========================================

This guide explains how to migrate from StepMania 5.1 to ITGmania without losing access to your settings and songs.

The steps are as follows:

1. Install ITGmania.
2. Copy over songs, settings, profiles and other user data. (See detailed instructions below.)
   If you don't intend on keeping StepMania 5.1 the data can be moved instead of copying.
3. (skip for portable builds) ITGmania ships with Simply Love and the Club Fantastic song packs included. Duplicates of them can be removed from the user data directory.
4. Uninstall StepMania 5.1 (optional).
   If you choose to do so you can also uninstall the GrooveStats launcher as ITGmania has network functionality built in and doesn't need an external tool for that. This is currently only supported by the Simply Love theme though, so users of other themes will still have to use the Launcher.
5. Make sure to enable GrooveStats integration in the operator menu if you want to use it.
6. You are all set!

If you want to keep StepMania 5.1 alongside ITGmania you can save some space by using the `AdditionalSongFolders` setting to point both installations to the same songs folder.

## Portable builds (all platforms)

Create a file after installation called Portable.ini at the root directory of the install to create a portable build.

Copy over `Announcers/`, `Characters/`, `Courses/`, `NoteSkins/` and `Songs/` to the ITGmania folder.
Make sure to remove any copy of the Simply Love theme from the `Themes` folder before copying it over. ITGmania ships with it's own version of Simply Love.

## Windows

Copy `%APPDATA%/StepMania 5.1/` to `%APPDATA%/ITGmania/`.

## macOS

On macOS the user data folder and the save folder are separate, so you have to copy both of them.

Copy `~/Library/Application Support/StepMania 5.1/` to `~/Library/Application Support/ITGmania/`.
Copy `~/Library/Preferences/StepMania 5.1/` to `~/Library/Preferences/ITGmania/`.

## Linux

Copy `~/.stepmania-5.1/` to `~/.itgmania/`.
