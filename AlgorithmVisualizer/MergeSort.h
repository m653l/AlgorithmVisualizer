#pragma once
#include "Algorithm.h"

namespace Algorithms {
	class MergeSort : public Algorithm
	{
	public:
		MergeSort();

		void step(Visualization::VisualizationData& data, SortingStats& stats) override;

		void run(Visualization::VisualizationData& data, SortingStats& stats) override;

		bool isComplete(const SortingStats& stats) const override;

		void reset() override;

	private:
		int i, j, temp = 0;
	};
}

