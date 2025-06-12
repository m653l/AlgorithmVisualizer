#include "MergeSort.h"
#include <thread>
#include <chrono>

namespace Algorithms {

    MergeSort::MergeSort() : Algorithm("Merge Sort") {
        reset();
    }

    void MergeSort::step(Visualization::VisualizationData& data, SortingStats& stats) {
        if (stats.sortingComplete || !stats.isSorting || running) return;

        running = true;
        data.resetHighlighting();

        mergeSort(data, stats, 0, data.size() - 1);

        running = false;
        stats.sortingComplete = true;
        stats.isSorting = false;
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
        mergeSort(data, stats, mid + 1, right);
        merge(data, stats, left, mid, right);
    }

    void MergeSort::merge(Visualization::VisualizationData& data, SortingStats& stats, int left, int mid, int right) {
        auto& array = data.getArray();

        std::vector<Visualization::ArrayElement> temp(right - left + 1);

        int i = left, j = mid + 1, k = 0;

        while (i <= mid && j <= right) {
            array[i].isComparing = true;
            array[j].isComparing = true;
            stats.comparisons++;

            if (array[i].value <= array[j].value) {
                temp[k++] = array[i++];
            }
            else {
                temp[k++] = array[j++];
            }

            stats.currentStep++;
            std::this_thread::sleep_for(std::chrono::milliseconds(stats.speedFactor));
        }

        while (i <= mid) {
            temp[k++] = array[i++];
            stats.currentStep++;
        }
        while (j <= right) {
            temp[k++] = array[j++];
            stats.currentStep++;
        }

        for (int t = 0; t < temp.size(); ++t) {
            array[left + t] = temp[t];
            array[left + t].isSwapping = true;
            stats.swaps++;
            std::this_thread::sleep_for(std::chrono::milliseconds(stats.speedFactor));
        }
    }

    bool MergeSort::isComplete(const SortingStats& stats) const {
        return stats.sortingComplete;
    }

    void MergeSort::reset() {
        completed = false;
        running = false;
    }

}
