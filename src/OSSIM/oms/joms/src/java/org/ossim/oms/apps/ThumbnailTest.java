package org.ossim.oms.apps;

import joms.oms.Init;
import joms.oms.Util;

public class ThumbnailTest
{
	public static void main(String[] args)
	{
		if ( args.length == 3 )
		{
			String[] newArgs = new String[args.length + 1];

			newArgs[0] = "org.ossim.oms.apps.ThumbnailTest";
			System.arraycopy(args, 0, newArgs, 1, args.length);
			Init.instance().initialize( newArgs.length, newArgs);

			String inputFile = args[0];
			int entryId = 0;
			String outputFile = args[1];
			String writerType = "image/jpeg";
			int xRes = Integer.parseInt(args[2]);
			int yRes = Integer.parseInt(args[2]);
			String histogramFile = "";
			String stretchType = "linear_auto_min_max";
			boolean keepAspectFlag = true;

			boolean status = Util.writeImageSpaceThumbnail(
				inputFile, entryId, outputFile, writerType, xRes, yRes, histogramFile, stretchType, keepAspectFlag
			);

			if ( !status )
			{
				System.err.println( "Error: cannot create thumbnail using args:");
				System.err.println("\tinputFile: " + inputFile);
				System.err.println("\tentryId: " + entryId);
				System.err.println("\toutputFile: " + outputFile);
				System.err.println("\twriterType: " + writerType);
				System.err.println("\txRes: " + xRes);
				System.err.println("\tyRes: " + yRes);
				System.err.println("\thistogramFile: " + histogramFile);
				System.err.println("\tstretchType: " + stretchType);
				System.err.println("\tkeepAspectFlag: " + keepAspectFlag);
				System.exit(-1);
			}
	    }
		else
		{
			System.out.println( "Usage: ThumbnailTest <input> <output> <size>");
		}
	}
}