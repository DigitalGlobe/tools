/***************************************************************************
    costcalculator.cpp
    ---------------------
    begin                : November 2009
    copyright            : (C) 2009 by Martin Dobias
    email                : wonder dot sk at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "layer.h"
#include "pal.h"
#include "feature.h"
#include "geomfunction.h"
#include "labelposition.h"
#include "util.h"
#include "costcalculator.h"
#include <cmath>
#include <cfloat>

using namespace pal;

bool CostCalculator::candidateSortGrow( const LabelPosition *c1, const LabelPosition *c2 )
{
  return c1->cost() < c2->cost();
}

bool CostCalculator::candidateSortShrink( const LabelPosition *c1, const LabelPosition *c2 )
{
  return c1->cost() > c2->cost();
}

void CostCalculator::addObstacleCostPenalty( LabelPosition* lp, FeaturePart* obstacle )
{
  int n = 0;
  double dist;
  double distlabel = lp->feature->getLabelDistance();

  switch ( obstacle->getGeosType() )
  {
    case GEOS_POINT:

      dist = lp->getDistanceToPoint( obstacle->x[0], obstacle->y[0] );
      if ( dist < 0 )
        n = 2;
      else if ( dist < distlabel )
        //note this never happens at the moment - points are not obstacles if they don't fall
        //within the label
        n = 1;
      else
        n = 0;
      break;

    case GEOS_LINESTRING:

      // Is one of label's borders crossing the line ?
      n = ( lp->crossesLine( obstacle ) ? 1 : 0 );
      break;

    case GEOS_POLYGON:
      // behaviour depends on obstacle avoid type
      switch ( obstacle->layer()->obstacleType() )
      {
        case QgsPalLayerSettings::PolygonInterior:
          // n ranges from 0 -> 12
          n = lp->polygonIntersectionCost( obstacle );
          break;
        case QgsPalLayerSettings::PolygonBoundary:
          // penalty may need tweaking, given that interior mode ranges up to 12
          n = ( lp->crossesBoundary( obstacle ) ? 6 : 0 );
          break;
        case QgsPalLayerSettings::PolygonWhole:
          // n is either 0 or 12
          n = ( lp->intersectsWithPolygon( obstacle ) ? 12 : 0 );
          break;
      }

      break;
  }

  if ( n > 0 )
    lp->setConflictsWithObstacle( true );

  //scale cost by obstacle's factor
  double obstacleCost = obstacle->obstacleFactor() * double( n );

  // label cost is penalized
  lp->setCost( lp->cost() + obstacleCost );
}

void CostCalculator::setPolygonCandidatesCost( int nblp, QList< LabelPosition* >& lPos, RTree<FeaturePart*, double, 2, double> *obstacles, double bbx[4], double bby[4] )
{
  double normalizer;
  // compute raw cost
  for ( int i = 0; i < nblp; ++i )
    setCandidateCostFromPolygon( lPos.at( i ), obstacles, bbx, bby );

  // lPos with big values came first (value = min distance from label to Polygon's Perimeter)
  // IMPORTANT - only want to sort first nblp positions. The rest have not had the cost
  // calculated so will have nonsense values
  QList< LabelPosition* > toSort;
  for ( int i = 0; i < nblp; ++i )
  {
    toSort << lPos.at( i );
  }
  qSort( toSort.begin(), toSort.end(), candidateSortShrink );
  for ( int i = 0; i < nblp; ++i )
  {
    lPos[i] = toSort.at( i );
  }

  // define the value's range
  double cost_max = lPos.at( 0 )->cost();
  double cost_min = lPos.at( nblp - 1 )->cost();

  cost_max -= cost_min;

  if ( cost_max > EPSILON )
  {
    normalizer = 0.0020 / cost_max;
  }
  else
  {
    normalizer = 1;
  }

  // adjust cost => the best is 0.0001, the worst is 0.0021
  // others are set proportionally between best and worst
  for ( int i = 0; i < nblp; ++i )
  {
    //if (cost_max - cost_min < EPSILON)
    if ( cost_max > EPSILON )
    {
      lPos.at( i )->setCost( 0.0021 - ( lPos.at( i )->cost() - cost_min ) * normalizer );
    }
    else
    {
      //lPos[i]->cost = 0.0001 + (lPos[i]->cost - cost_min) * normalizer;
      lPos.at( i )->setCost( 0.0001 );
    }
  }
}

void CostCalculator::setCandidateCostFromPolygon( LabelPosition* lp, RTree <FeaturePart*, double, 2, double> *obstacles, double bbx[4], double bby[4] )
{
  double amin[2];
  double amax[2];

  PolygonCostCalculator *pCost = new PolygonCostCalculator( lp );

  // center
  //cost = feat->getDistInside((this->x[0] + this->x[2])/2.0, (this->y[0] + this->y[2])/2.0 );

  pCost->update( lp->feature );

  PointSet *extent = new PointSet( 4, bbx, bby );

  pCost->update( extent );

  delete extent;

  lp->feature->getBoundingBox( amin, amax );

  obstacles->Search( amin, amax, LabelPosition::polygonObstacleCallback, pCost );

  lp->setCost( pCost->getCost() );

  delete pCost;
}

int CostCalculator::finalizeCandidatesCosts( Feats* feat, int max_p, RTree <FeaturePart*, double, 2, double> *obstacles, double bbx[4], double bby[4] )
{
  // If candidates list is smaller than expected
  if ( max_p > feat->lPos.count() )
    max_p = feat->lPos.count();
  //
  // sort candidates list, best label to worst
  qSort( feat->lPos.begin(), feat->lPos.end(), candidateSortGrow );

  // try to exclude all conflitual labels (good ones have cost < 1 by pruning)
  double discrim = 0.0;
  int stop;
  do
  {
    discrim += 1.0;
    for ( stop = 0; stop < feat->lPos.count() && feat->lPos.at( stop )->cost() < discrim; stop++ )
      ;
  }
  while ( stop == 0 && discrim < feat->lPos.last()->cost() + 2.0 );

  if ( discrim > 1.5 )
  {
    int k;
    for ( k = 0; k < stop; k++ )
      feat->lPos.at( k )->setCost( 0.0021 );
  }

  if ( max_p > stop )
    max_p = stop;

  // Sets costs for candidates of polygon

  if ( feat->feature->getGeosType() == GEOS_POLYGON )
  {
    int arrangement = feat->feature->layer()->arrangement();
    if ( arrangement == QgsPalLayerSettings::Free || arrangement == QgsPalLayerSettings::Horizontal )
      setPolygonCandidatesCost( stop, feat->lPos, obstacles, bbx, bby );
  }

  // add size penalty (small lines/polygons get higher cost)
  feat->feature->addSizePenalty( max_p, feat->lPos, bbx, bby );

  return max_p;
}

PolygonCostCalculator::PolygonCostCalculator( LabelPosition *lp ) : lp( lp )
{
  px = ( lp->x[0] + lp->x[2] ) / 2.0;
  py = ( lp->y[0] + lp->y[2] ) / 2.0;

  dist = DBL_MAX;
  ok = false;
}

void PolygonCostCalculator::update( PointSet *pset )
{
  double d = pset->minDistanceToPoint( px, py );
  if ( d < dist )
  {
    dist = d;
  }
}

LabelPosition* PolygonCostCalculator::getLabel()
{
  return lp;
}

double PolygonCostCalculator::getCost()
{
  return ( 4 * dist );
}
