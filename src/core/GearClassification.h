/* SchemaEditor.cpp
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable
 * This file is part of Drone.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef GEARCLASSIFICATION_INCLUDED
#define GEARCLASSIFICATION_INCLUDED

#include <string>
#include <vector>

/**
 * abstract classification that we have in the GearInfo structure
 * inherited by GearClassificationT
 */
class GearClassification
{
public:  
  virtual std::string toString()=0;  
  virtual std::vector<std::string> path()=0;
};

/**
 * Representation of a classification
 * Me must be the class that inherit ClassificationT
 * ParentNode must be the parent classification in the tree structure.
 */
template<typename Me, typename ParentNode>
class GearClassificationT : public GearClassification
{
public:  
    
  /**
   * return a vector representing the full path to this classication 
   * from the root.
   * 
   * @return return a vector representing the full path to this classication 
   *         from the root.
   */
  virtual std::vector<std::string> path()
  {    
    std::vector<std::string> tmpPath;    
    
    //ask the parent to give is path, this will cause a recursion 
    //to the root classification.
    std::vector<std::string> parentPath = parent().path();

    tmpPath.insert(tmpPath.end(), parentPath.begin(), parentPath.end());
    tmpPath.push_back(toString());
        
    return tmpPath;
  }
  
  /**
   * return a static instance of this classification
   * 
   * @return return a static instance of this classification
   */
  Me* instance()
  { 
    //since we are in a template, each classification will have only one instance
    static Me me;            
    return &me;    
  }

private:
  ParentNode parent(){return ParentNode();}  
};

/**
 * Provide a compile time tree structure of classification for gears.
 * The structure is programmaticly represented by an inner classing structure.
 * Each classification is of type ClassificationT<Me, ParentNode>.
 */
class GearClassifications
{
public:
  //ROOT
  class Root : public GearClassificationT<Root, Root>
  { 
    public: 
      std::string toString(){return "Root";}
      //Root overload the path method, to return an empty vector
      //it's very important for the recursive path mechanism because it's actually the only classification
      //to return an instance of a vector, on which the full path is appended      
      virtual std::vector<std::string> path()
      {
        //here we return an empty vector because we dont want the root to be visible in the path         
        return std::vector<std::string>();
      }
  };

  //VIDEO
  class GearClassification_Video: public GearClassificationT<GearClassification_Video, Root>
  {
  public: 
    std::string toString(){return "Video";} 

    //VIDEO/IO
    class GearClassification_IO: public GearClassificationT<GearClassification_IO, GearClassification_Video>
    {
    public: std::string toString(){return "IO";} 
    };
    GearClassification_IO IO(){return GearClassification_IO();}  

    //VIDEO/Time
    class GearClassification_Time: public GearClassificationT<GearClassification_Time, GearClassification_Video>
    {
    public: std::string toString(){return "Time";}       
    };       
    GearClassification_Time time(){return GearClassification_Time();}  
    
    //VIDEO/Blur
    class GearClassification_Blur: public GearClassificationT<GearClassification_Blur, GearClassification_Video>
    {
    public: std::string toString(){return "Blur";}       
    };       
    GearClassification_Blur blur(){return GearClassification_Blur();}  

    //VIDEO/Distortion
    class GearClassification_Distortion: public GearClassificationT<GearClassification_Distortion, GearClassification_Video>
    {
    public: std::string toString(){return "Distortion";}       
    };       
    GearClassification_Distortion distortion(){return GearClassification_Distortion();}  

    //VIDEO/Color
    class GearClassification_Color: public GearClassificationT<GearClassification_Color, GearClassification_Video>
    {
    public: std::string toString(){return "Color";}       
    };       
    GearClassification_Color color(){return GearClassification_Color();}  

    //VIDEO/Composition
    class GearClassification_Composition: public GearClassificationT<GearClassification_Composition, GearClassification_Video>
    {
    public: std::string toString(){return "Composition";}       
    };       
    GearClassification_Composition composition(){return GearClassification_Composition();}  

    //VIDEO/FeatureExtraction
    class GearClassification_FeatureExtraction: public GearClassificationT<GearClassification_FeatureExtraction, GearClassification_Video>
    {
    public: std::string toString(){return "Feature Extraction";}       
    };       
    GearClassification_FeatureExtraction featureExtraction(){return GearClassification_FeatureExtraction();}  

  };
  
  //SIGNAL
  class GearClassification_Signal: public GearClassificationT<GearClassification_Signal, Root>
  {
  public: 
    std::string toString(){return "Signal";} 

    //SIGNAL/IO
    class GearClassification_IO: public GearClassificationT<GearClassification_IO, GearClassification_Signal>
    {
    public: std::string toString(){return "IO";} 
    };
    GearClassification_IO IO(){return GearClassification_IO();}     
    
    //SIGNAL/Generator
    class GearClassification_Generator: public GearClassificationT<GearClassification_Generator, GearClassification_Signal>
    {
    public: std::string toString(){return "Generator";} 
    };    
    GearClassification_Generator generator(){return GearClassification_Generator();}     

  };

  //CONTROL
  class GearClassification_Control: public GearClassificationT<GearClassification_Control, Root>
  {
  public: 
    std::string toString(){return "Control";}     
  };

  //UNCLASSIFIED
  class GearClassification_Unclassified: public GearClassificationT<GearClassification_Unclassified, Root>
  {
  public: 
    std::string toString(){return "Unclassified";}     
  };


  //root classifications
  static GearClassification_Video video(){return GearClassification_Video();}    
  static GearClassification_Signal signal(){return GearClassification_Signal();}    
  static GearClassification_Control control(){return GearClassification_Control();}    
  static GearClassification_Unclassified unclassified(){return GearClassification_Unclassified();}    

};

#endif

