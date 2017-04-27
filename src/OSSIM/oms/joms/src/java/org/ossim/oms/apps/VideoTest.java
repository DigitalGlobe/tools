package org.ossim.oms.apps;
import joms.oms.Init;
import joms.oms.Video;

public class VideoTest
	{
		/**
		 * @param args
		 */
		public static void main( String[] args )
		{
			String[] newArgs = new String[0];
			Init.instance().initialize( newArgs.length, newArgs);
			Video video = new Video();
			if ( args.length == 4 )
			{
				if(video.open(args[0]))
				{
					video.nextFrame();
					video.writeCurrentFrameToFile(args[1],
															Integer.valueOf(args[3]),
															args[2]);
				}
			}
			else if( args.length == 3)
			{
				if(video.open(args[0]))
				{
					video.nextFrame();
					video.writeCurrentFrameToFile(args[1],Integer.valueOf(args[2]));
				}
			}
			else
			{
				System.out.println("Usage: VideoTest <inputfile> <output> <resolution> [<mime type>]");
			}
		}
	}
