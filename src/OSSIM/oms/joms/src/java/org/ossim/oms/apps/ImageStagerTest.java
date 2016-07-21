package org.ossim.oms.apps;
import joms.oms.Init;
import joms.oms.ImageStager;
import joms.oms.Util;
import joms.oms.UintVector;
import java.io.File;

public class ImageStagerTest
{
	/**
	 * @param args
	 */
	public static void main( String[] args )
	{
		Init.instance().initialize();
		if ( args.length > 0 )
		{
			for ( int i = 0; i < args.length; i++ )
			{
				File file = new File(args[i]);
            UintVector entryIds = new UintVector();
            Util.getEntryList(entryIds, file.getAbsolutePath());
            if(entryIds.isEmpty() == false)
            {
               ImageStager stager = new ImageStager();
               if(stager.open(file.getAbsolutePath()) == true)
               {
                  
                  for(int entryIdx = 0; entryIdx < entryIds.size(); ++entryIdx)
                  {
                     stager.setEntry(entryIds.get(entryIdx));
                     stager.setDefaults();
                     stager.stage();
                  }
               }
            }
			}
		}
	}
}
