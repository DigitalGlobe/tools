itcl_class WKB_DataDictionary {
	inherit dd

constructor {wpath location} {
#	dd::constructor $windowpath $location
    dd $wpath $location
}

method layout {} {
    frame $window.center
	mapListbox
	buttons
	updatedict
	::update idletasks	
}

method buttons {} {
	return
}

method getListEntry {index} {
	regsub {\([A-Z]+\)$} [$mapList subwidget listbox get $index] {} s_name 
	return $s_name
}

method updatedict {} {

  $mapList subwidget listbox delete 0 [$mapList subwidget listbox size]
  foreach s_item [ecs_UpdateDictionary $url] {
  $mapList subwidget listbox insert end [getURLEntry $s_item]
  }

}

method getURLEntry {s_path} {
	return ${s_path}
}

method getSelectedClass {args} {
 	return Raster
}

method getPathname {coverage class selection} {
	return $selection	
}

protected a_ramdisk ;# whether the coverage is to be loaded into ram or disk.
					;# values are either "RAM" or "DISK"
protected s_diskDefault "DISK"

}
