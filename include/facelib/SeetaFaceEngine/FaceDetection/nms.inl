
#include "nms.h"

#include <algorithm>
#include <cstdint>
#include <vector>

namespace seeta {
namespace fd {

bool CompareBBox(const seeta::FaceInfo & a, const seeta::FaceInfo & b) {
  return a.score > b.score;
}

void NonMaximumSuppression(std::vector<seeta::FaceInfo>* bboxes,
  std::vector<seeta::FaceInfo>* bboxes_nms, float iou_thresh) {
  bboxes_nms->clear();
  std::sort(bboxes->begin(), bboxes->end(), seeta::fd::CompareBBox);

  int32_t select_idx = 0;
  int32_t num_bbox = static_cast<int32_t>(bboxes->size());
  std::vector<int32_t> mask_merged(num_bbox, 0);
  bool all_merged = false;

  while (!all_merged) {
    while (select_idx < num_bbox && mask_merged[select_idx] == 1)
      select_idx++;
    if (select_idx == num_bbox) {
      all_merged = true;
      continue;
    }

    bboxes_nms->push_back((*bboxes)[select_idx]);
    mask_merged[select_idx] = 1;

    seeta::Rect select_bbox = (*bboxes)[select_idx].bbox;
    float area1 = static_cast<float>(select_bbox.width * select_bbox.height);
    float x1 = static_cast<float>(select_bbox.x);
    float y1 = static_cast<float>(select_bbox.y);
    float x2 = static_cast<float>(select_bbox.x + select_bbox.width - 1);
    float y2 = static_cast<float>(select_bbox.y + select_bbox.height - 1);

    select_idx++;
    for (int32_t i = select_idx; i < num_bbox; i++) {
      if (mask_merged[i] == 1)
        continue;

      seeta::Rect & bbox_i = (*bboxes)[i].bbox;
      float x = MAX(x1, static_cast<float>(bbox_i.x));
      float y = MAX(y1, static_cast<float>(bbox_i.y));
      float w = MIN(x2, static_cast<float>(bbox_i.x + bbox_i.width - 1)) - x + 1;
      float h = MIN(y2, static_cast<float>(bbox_i.y + bbox_i.height - 1)) - y + 1;
      if (w <= 0 || h <= 0)
        continue;

      float area2 = static_cast<float>(bbox_i.width * bbox_i.height);
      float area_intersect = w * h;
      float area_union = area1 + area2 - area_intersect;
      if (static_cast<float>(area_intersect) / area_union > iou_thresh) {
        mask_merged[i] = 1;
        bboxes_nms->back().score += (*bboxes)[i].score;
      }
    }
  }
}

}  // namespace fd
}  // namespace seeta
