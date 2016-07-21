
TEMPLATE	= subdirs
LANGUAGE	= C++

# depend on variables, not actual directories
SUBDIRS = sub_src sub_apps

# specify the directories and dependencies
sub_src.subdir = src
sub_apps.subdir = apps
sub_apps.depends = sub_src
