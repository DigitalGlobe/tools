#ifndef omsImageStager_HEADER
#define omsImageStager_HEADER 1
#include <oms/Constants.h>
#include <oms/Object.h>
#include <string>

namespace oms
{
	class OMSDLL ImageStager : public oms::Object
	{
	public:
		class PrivateData;
		
		/**
		 *
		 * Takes an input file to stage as input.  If you don't supply one you will need to call
		 * setFilename to specify the file to stage.  You can also give an entry id.  If the 
       * entry id is < 0 then all entries are done for theat input file.
		 *
		 */
		ImageStager();//const std::string& filename="", ossim_int32 entryId=-1);
		
		/**
		 * Destructor
		 */
		virtual ~ImageStager();
		
		/**
		 * Sets the file to stage and opens the file
       *
       * @param filename the file to stage
		 */
		bool open(const std::string& filename);
		
      unsigned int getNumberOfEntries()const;
      
      bool setEntry(ossim_uint32 entryId);
      
      /**
       * Returns the information for the current entry
       */
      std::string getInfo(bool failIfNoGeometryFlag=true)const;
      
      /**
       * Returns all information for all entries
       */
      std::string getAllInfo(bool failIfNoGeometryFlag=true)const;
      
      int getCurrentEntry()const;
		/**
		 * Returns the current filename.
		 */
		std::string getFilename()const;
		
      /**
       * This will set a flag to turn on fast histogram staging.  This wil use a small random
       * set of tiles to calculate a histogram instead of scanning the entire image.
       */
      void setUseFastHistogramStagingFlag(bool flag);
      
		/**
		 * You can set the staging flag for histograms.  This allows you to manual 
		 * override and force a true or false setting.
		 */
		void setHistogramStagingFlag(bool flag);
		
		/**
		 * You can set the staging flag for overviews.  This allows you to manual override and
		 * force a true or false setting.
		 */
		void setOverviewStagingFlag(bool flag);

		void setOverviewType(const std::string& overviewType);
		
		void setCompressionType(const std::string& compressionType);

		void setCompressionQuality(int compressionQuality);
		/**
		 * When setDaults is called it will automatically init the histogram filename to a default location.
		 * if you want to override that location you can manually set the histogram filename.
		 */
		void setHistogramFilename(const std::string& file);
		
		/**
		 * @return the current histogram filename
		 */
		std::string getHistogramFilename()const;
		
		/**
		 * When setDefaults is called it will automatically init the overview filename to a default location.
		 * if you want to override that location you can manually set the histogram filename.
		 */
		void setOverviewFilename(const std::string& file);

		/**
		 * @return the current histogram filename
		 */
		std::string getOverviewFilename()const;
		
		void disableAll();
		
		/**
		 * Will look at the input file and try to determine what needs to be staged and enable the flag automatically.  It will also 
		 * set the default locations to put the histogram file and opverview files, ... etc.
		 */ 
		void setDefaults();
		
      void setOverviewTileSize(int width, int height);
      
		/**
		 * Will begin the staging process.  One entry at a time.
		 */
		bool stage();
		
		/**
		 * Will stage all entries of image data set
		 */
		bool stageAll();
      
		
      bool hasOverviews()const;
      bool hasHistograms()const;
      
      bool buildOverviews();
      bool buildAllOverviews();
      bool buildHistograms();
      bool buildAllHistograms();
		/**
		 * This will allow you to cancel the staging process
		 */
		void cancel();
		
	protected:
		PrivateData* thePrivateData;
	};
}

#endif
