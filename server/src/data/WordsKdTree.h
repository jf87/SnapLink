#pragma once

#include <opencv2/core/core.hpp>
#include <flann/flann.hpp>
#include "data/Words.h"

class WordsKdTree : public Words
{
public:
    WordsKdTree();

    /**
     * Add words
     */
    void putWords(std::list< std::unique_ptr<rtabmap::VisualWord> > &&words);

    /**
     * find the indices of the nearst neighbors of descriptors
     */
    std::vector<int> findNNs(const cv::Mat &descriptors) const;

private:
    void build();

private:
    int _type;
    int _dim;
    std::list< std::unique_ptr<rtabmap::VisualWord> > _words;
    cv::Mat _dataMat;
    std::unique_ptr< flann::Index< flann::L2<float> > > _index;
    std::map<int , int> _mapIndexId;
};