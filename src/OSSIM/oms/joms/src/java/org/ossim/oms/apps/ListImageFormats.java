package org.ossim.oms.apps;

import joms.oms.Init;
import joms.oms.ossimImageWriterFactoryRegistry;
import joms.oms.ossimStringVector;

public class ListImageFormats
{
	static
	{
		System.loadLibrary( "joms" );
	}

	/**
	 * @param args
	 */
	public static void main( String[] args )
	{
		Init.instance().initialize( args.length, args );

		ossimStringVector typeList = new ossimStringVector();

		ossimImageWriterFactoryRegistry.instance().getTypeNameList( typeList );

		for ( int i = 0; i < typeList.size(); i++ )
		{
			System.out.println( typeList.get( i ) );
		}
	}

}
