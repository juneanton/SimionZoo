#include "single-dimension-grid.h"
#include "features.h"
#include "named-var-set.h"
#include <algorithm>

SingleDimensionGrid::SingleDimensionGrid(size_t numValues, double min, double max, bool circular)
{
	m_min = min;
	m_max = max;
	m_rangeWidth = max - min;
	m_bCircular = circular;

	m_values = vector<double>(numValues);
	for (int i = 0; i < numValues; i++)
		m_values[i] = m_min + (((double)i) / (numValues - 1))*(m_rangeWidth);
}

SingleDimensionGrid::~SingleDimensionGrid()
{
}


size_t SingleDimensionGrid::getClosestFeature(double value) const
{
	unsigned int nearestIndex = 0;
	double dist, minDist;

	if (!m_bCircular)
	{
		minDist = abs(value - m_values[0]);
		for (unsigned int i = 1; i < m_values.size(); i++)
		{
			dist = abs(value - m_values[i]);
			if ( dist < minDist )
			{
				nearestIndex = i;
				minDist = dist;
			}
		}
	}
	else
	{
		minDist = std::min(abs(value - m_values[0]), abs(value + m_rangeWidth - m_values[0]));
		for (unsigned int i = 1; i < m_values.size(); i++)
		{
			dist = abs(value - m_values[i]);
			if (dist < minDist)
			{
				nearestIndex = i;
				minDist = dist;
			}
		}
	}

	return nearestIndex;
}

double SingleDimensionGrid::getFeatureValue(size_t feature) const
{
	return m_values[feature];
}