# RAID_Management
Redundant Array of Independent Disks as software implementation using ipc and message passing in c language.

- Here, RAID 4 is implemented.

##How it works

- Because reading from 4 virtual disk(folder) is parallel, mesage passing is used here.

- First file is a seperator file. it seperates a given file onto 4 virtual disks(folders).
- Second file is combiner file which combine the original file from those 4 virtual disks.
