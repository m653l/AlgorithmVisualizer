#pragma once
#include "VisualizationData.h"
#include "SortingStats.h"
#include <string>

namespace Algorithms {
	class Algorithm
	{
    public:
        Algorithm(const std::string& name);
        virtual ~Algorithm() = default;

        // Get algorithm name
        const std::string& getName() const { return name; }

        // Execute one step of the algorithm
        virtual void step(Visualization::VisualizationData& data, SortingStats& stats) = 0;

        // Run the complete algorithm
        virtual void run(Visualization::VisualizationData& data, SortingStats& stats) = 0;

        // Check if the algorithm has completed
        virtual bool isComplete(const SortingStats& stats) const = 0;

        // Reset the algorithm state
        virtual void reset() = 0;

    protected:
        std::string name;
	};
}

