#include "MergeSort.h"
#include <thread>
#include <chrono>

namespace Algorithms {

    MergeSort::MergeSort() : Algorithm("Merge Sort") {
        reset();
    }

    void MergeSort::reset() {
        completed = false;
    }

    bool MergeSort::isComplete(const SortingStats& stats) const {
        return stats.sortingComplete;
    }

    void MergeSort::run(Visualization::VisualizationData& data, SortingStats& stats) {
        if (stats.currentStep == 0) {
            reset();
            stats.reset();
        }

        stats.isSorting = true;

        mergeSort(data, stats, 0, data.size() - 1);

        data.resetHighlighting();
        stats.sortingComplete = true;
        stats.isSorting = false;
    }

    void MergeSort::mergeSort(Visualization::VisualizationData& data, SortingStats& stats, int left, int right) {
        if (!stats.isSorting || left >= right) return;

        int mid = left + (right - left) / 2;

        mergeSort(data, stats, left, mid);
        if (!stats.isSorting) return;

        mergeSort(data, stats, mid + 1, right);
        if (!stats.isSorting) return;

        merge(data, stats, left, mid, right);
    }

    void MergeSort::merge(Visualization::VisualizationData& data, SortingStats& stats, int left, int mid, int right) {
        auto& array = data.getArray();
        std::vector<Visualization::ArrayElement> temp;

        int i = left, j = mid + 1;

        while (i <= mid && j <= right && stats.isSorting) {
            data.resetHighlighting();

            array[i].isComparing = true;
            array[j].isComparing = true;
            stats.comparisons++;

            if (array[i].value <= array[j].value) {
                temp.push_back(array[i++]);
            }
            else {
                temp.push_back(array[j++]);
            }

            stats.currentStep++;
            std::this_thread::sleep_for(std::chrono::milliseconds(stats.speedFactor));
        }

        while (i <= mid && stats.isSorting) {
            temp.push_back(array[i++]);
        }

        while (j <= right && stats.isSorting) {
            temp.push_back(array[j++]);
        }

        stats.currentStep++;
     
        for (int k = 0; k < temp.size(); ++k) {
            if (array[left + k].value != temp[k].value) {
                array[left + k].isSwapping = true;
                stats.swaps++;
            }
            array[left + k] = temp[k];
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(stats.speedFactor));
    }

}
