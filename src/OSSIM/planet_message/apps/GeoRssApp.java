import java.net.URL;
import java.util.List;
import java.util.Iterator;
import java.io.InputStreamReader;
import com.sun.syndication.feed.synd.SyndFeed;
import com.sun.syndication.feed.synd.SyndEntry;
import com.sun.syndication.io.SyndFeedInput;
import com.sun.syndication.io.XmlReader;
import com.sun.syndication.feed.module.georss.*;
import org.ossim.planet.message.*;

/**
 * It Reads and prints any RSS/Atom feed type.
 * <p>
 * @author Alejandro Abdelnur
 *
 */
public class GeoRssApp {
   // At the time of this document you will need
   // http://georss.geonames.org/georss-rome-0.9.8.jar
   // http://georss.geonames.org/rome-0.9.jar
   // http://georss.geonames.org/jdom-1.0.jar
   // for the example goerss read.
   //
   // Args: <ip> <port> <georss url>
   //
   // Sample input urls
   // http://local.yahooapis.com/MapsService/rss/trafficData.xml?appid=YahooDemo&city=new+york
   // http://ws.geonames.org/rssToGeoRSS?feedUrl=today.reuters.com/rss/worldNews
   // http://earthquake.usgs.gov/eqcenter/catalogs/eqs7day-M5.xml
   //
   public static void main(String[] args) 
   {
      boolean ok = false;
      if (args.length==3) 
      {
         String ip = args[0];
         String port = args[1];
         String url = args[2];
         try {
            URL feedUrl = new URL(url);
            
            SyndFeedInput input = new SyndFeedInput();
            SyndFeed feed = input.build(new XmlReader(feedUrl));
            
            //System.out.println(feed);
            List<SyndEntry> entries = feed.getEntries();
            Iterator<SyndEntry> iter = entries.iterator();
            while(iter.hasNext()) 
            {
               SyndEntry entry = iter.next();
               if(entry != null)
               {
                  GeoRSSModule geoRSSModule = GeoRSSUtils.getGeoRSS(entry);
                  if(geoRSSModule != null)
                  {
                     Action a = new Action(Action.Type.ADD);
                     Placemark p = new Placemark();
                     p.setGeometry(new Point(geoRSSModule.getPosition().getLongitude(),
                                             geoRSSModule.getPosition().getLatitude() ,
                                             0.0,
                                             "clampToGround"));
                     p.setName(entry.getTitle());
                     p.setDescription(entry.getDescription().getValue());
                     p.setId(entry.getTitle());
                     a.setTargetAttribute(":annotation");
                     a.addChild(p);
                     Message m = new Message();
                     m.add(a);
                     m.sendMessageToSocket(ip, Integer.valueOf(port), true, 10);
                     
                     System.out.println(entry.getTitle() + " : lat="
                                        + geoRSSModule.getPosition().getLatitude() + ",lng="
                                        + geoRSSModule.getPosition().getLongitude() + ", desc="
                                        + entry.getDescription().getValue() + "; time="
                                        + entry.getPublishedDate());
                     System.out.println("______________________________________________________________");
                  }
               }
            }            
            ok = true;
         }
         catch (Exception ex) {
            ex.printStackTrace();
            System.out.println("ERROR: "+ex.getMessage());
         }
      }
      
      if (!ok) {
         System.out.println();
         System.out.println("FeedReader reads and prints any RSS/Atom feed type.");
         System.out.println("The first parameter must be the URL of the feed to read.");
         System.out.println();
      }
   }
   
}