import  xml.etree.ElementTree as ET


class XmlUtils:
    def __init__(self) :
        pass

    def _indent(self, elem, level=0, more_sibs=False):
        i = "\n"
        if level:
            i += (level-1) * '  '
        num_kids = len(elem)
        if num_kids:
            if not elem.text or not elem.text.strip():
                elem.text = i + "  "
                if level:
                    elem.text += '  '
            count = 0
            for kid in elem:
                self._indent(kid, level+1, count < num_kids - 1)
                count += 1
            if not elem.tail or not elem.tail.strip():
                elem.tail = i
                if more_sibs:
                    elem.tail += '  '
        else:
            if level and (not elem.tail or not elem.tail.strip()):
                elem.tail = i
                if more_sibs:
                    elem.tail += '  '

    def buildDll(self, configuration, platform, compileprops, libprops, filename):
        project = ET.Element('Project')
        project.set('xmlns', "http://schemas.microsoft.com/developer/msbuild/2003")
        idg = ET.SubElement(project, 'ItemDefinitionGroup')
        idg.set('Condition', "'$(Configuration)|$(Platform)'=='" + configuration + '|' + platform + "'")
        comp = ET.SubElement(idg, 'ClCompile')
        link = ET.SubElement(idg, 'Link')
        
        for k in compileprops:
          prop = ET.SubElement(comp, k)
          prop.text = compileprops[k]

        for k in libprops:
          prop = ET.SubElement(link, k)
          prop.text = libprops[k]
        
        self._indent(project)
        tree = ET.ElementTree(project)
        tree.write(filename)
        
    def buildLib(self, configuration, platform, compileprops, libprops, filename):
        project = ET.Element('Project')
        project.set('xmlns', "http://schemas.microsoft.com/developer/msbuild/2003")
        idg = ET.SubElement(project, 'ItemDefinitionGroup')
        idg.set('Condition', "'$(Configuration)|$(Platform)'=='" + configuration + '|' + platform + "'")
        comp = ET.SubElement(idg, 'ClCompile')
        lib = ET.SubElement(idg, 'Lib')
        
        for k in compileprops:
          prop = ET.SubElement(comp, k)
          prop.text = compileprops[k]
        
        for k in libprops:
          prop = ET.SubElement(lib, k)
          prop.text = libprops[k]
        
        self._indent(project)
        tree = ET.ElementTree(project)
        tree.write(filename)
        
        
