import org.ossim.planet.message.*;
import java.io.*;

class GroupTest
{
	public static void main(String[] args)
	{
		Group group = new Group();
		
		group.add(new Placemark());
		group.add(new Image());
		ByteArrayOutputStream memoryStream = new ByteArrayOutputStream();
		XMLEncoder encoder = new XMLEncoder(memoryStream);
		
		encoder.writeObject(group);
		ByteArrayInputStream in = new ByteArrayInputStream(memoryStream.toByteArray());
		
		XMLDecoder decoder = new XMLDecoder(in);
		BaseObject obj = decoder.readObject();
	}
}
