package org.ossim.oms.apps;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Date;

import joms.oms.DataInfo;
import joms.oms.Init;

public class ImageFinder
{
  protected void finalize() throws Throwable
  {
    super.finalize();
    all.close();
    skipped.close();
    success.close();
    failure.close();
  }

  protected DataInfo imageInfo = new DataInfo();

  protected PrintWriter all;

  protected PrintWriter skipped;

  protected PrintWriter success;

  protected PrintWriter failure;

  public ImageFinder()
  {
    try
    {
      all = new PrintWriter( new FileWriter( "all.txt" ) );
      skipped = new PrintWriter( new FileWriter( "skipped.txt" ) );
      success = new PrintWriter( new FileWriter( "success.txt" ) );
      failure = new PrintWriter( new FileWriter( "failure.txt" ) );
    }
    catch ( IOException e )
    {
      // TODO Auto-generated catch block
      e.printStackTrace();
    }

  }

  public boolean checkFile( File file )
  {
    boolean status = true;

    if ( file.isFile() )
    {
      String[] patterns = {
          "(ovr|his|geom|omd|prj|spec|kwl|statistics|hdr)", "(dbf|shx)", "bin",
          ".*\\~", "ds_store",
          "(htaccess|pdf|xml|xsl|xls|ppt|txt|rtf|doc|readme|htm[l]?)",
          "(zip|tar|gz|tgz)", "(tfw|jpw|pgw|nfw|sdw|jgw)", "(cpp|h|o|obj)",
          "(java|class|jar|war|groovy)", "(dll|so|exe|sh)"
      };

      String target = file.getAbsolutePath().toLowerCase();

      for ( int i = 0; i < patterns.length; i++ )
      {
        if ( target.matches( ".*" + patterns[i] ) )
        {
          status = false;
          break;
        }
      }
    }
    else if ( file.isDirectory() )
    {
      status = false;
    }
    return status;
  }

  public void scanFile( File file )
  {
    all.println( file );

    if ( checkFile( file ) )
    {
      if ( imageInfo.open( file.getAbsolutePath() ) )
      {
        success.println( file );
      }
      else
      {
        failure.println( file );
      }

      imageInfo.close();
    }
    else
    {
      skipped.println( file );
    }
  }

  public void scanDirectory( File dir )
  {
    File[] files = dir.listFiles();
    File noScan = new File( dir, "__OSSIM_NO_SCAN__" );

    if ( !noScan.exists() )
    {
      File atoc1 = new File( dir, "a.toc" );
      File atoc2 = new File( dir, "A.TOC" );
      File dht = new File( dir, "dht" );

      if ( atoc1.exists() )
      {
        scanFile( atoc1 );
      }
      else if ( atoc2.exists() )
      {
        scanFile( atoc2 );
      }
      else if ( dht.exists() )
      {
        scanFile( dht );
      }
      else
      {
        for ( int i = 0; files != null && i < files.length; i++ )
        {
          scan( files[i] );
        }
      }
    }
  }

  public void scan( File fileOrDir )
  {
    if ( fileOrDir.isFile() )
    {
      scanFile( fileOrDir );
    }
    else if ( fileOrDir.isDirectory() )
    {
      scanDirectory( fileOrDir );
    }
  }

  /**
   * @param args
   * @throws Throwable
   */
  public static void main( String[] args ) throws Throwable
  {
    Init.instance().initialize( args.length, args );

	if ( args.length > 0 )
	{
		for ( int i = 0; i < args.length; i++ )
		{
			ImageFinder imageFinder = new ImageFinder();
			File fileOrDir = new File( args[i] );

			System.out.println( "Start: " + new Date() );
			imageFinder.scan( fileOrDir );
			imageFinder.finalize();
			System.out.println( " Stop: " + new Date() );
		}
	}
	else
	{
		System.err.println( "Must specify at least one file or directory" );
	}
  }

}
