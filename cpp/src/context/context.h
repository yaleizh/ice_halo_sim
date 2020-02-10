#ifndef SRC_CONTEXT_CONTEXT_H_
#define SRC_CONTEXT_CONTEXT_H_

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "context/camera_context.h"
#include "context/crystal_context.h"
#include "context/multi_scatter_context.h"
#include "context/render_context.h"
#include "context/sun_context.h"
#include "core/crystal.h"
#include "core/optics.h"
#include "io/file.h"
#include "io/serialize.h"
#include "rapidjson/document.h"


namespace icehalo {

constexpr int kInvalidId = std::numeric_limits<int>::lowest();

class ProjectContext;
enum class LensType;
enum class VisibleRange;

using ProjectContextPtrU = std::unique_ptr<ProjectContext>;
using ProjectContextPtr = std::shared_ptr<ProjectContext>;


enum Symmetry : uint8_t {
  kSymmetryNone = 0u,
  kSymmetryPrism = 1u,
  kSymmetryBasal = 2u,
  kSymmetryDirection = 4u,
};


size_t RayPathHash(const std::vector<uint16_t>& ray_path, bool reverse = false);
size_t RayPathHash(const Crystal* crystal, const RaySegment* last_ray, int length, bool reverse = false);


class AbstractRayPathFilter {
 public:
  AbstractRayPathFilter();
  virtual ~AbstractRayPathFilter() = default;

  bool Filter(const Crystal* crystal, RaySegment* last_r) const;

  void SetSymmetryFlag(uint8_t symmetry_flag);
  void AddSymmetry(Symmetry symmetry);
  uint8_t GetSymmetryFlag() const;
  virtual void ApplySymmetry(const Crystal* crystal);

  void EnableComplementary(bool enable);
  bool GetComplementary() const;

  void EnableRemoveHomodromous(bool enable);
  bool GetRemoveHomodromous() const;

 protected:
  virtual bool FilterPath(const Crystal* crystal, RaySegment* last_r) const = 0;

  uint8_t symmetry_flag_;
  bool complementary_;
  bool remove_homodromous_;
};

using RayPathFilterPtrU = std::unique_ptr<AbstractRayPathFilter>;


class NoneRayPathFilter : public AbstractRayPathFilter {
 protected:
  bool FilterPath(const Crystal* crystal, RaySegment* last_r) const override;
};


class SpecificRayPathFilter : public AbstractRayPathFilter {
 public:
  void AddPath(const std::vector<uint16_t>& path);
  void ClearPaths();

  void ApplySymmetry(const Crystal* crystal) override;

 protected:
  bool FilterPath(const Crystal* crystal, RaySegment* last_r) const override;

 private:
  std::unordered_set<size_t> ray_path_hashes_;
  std::vector<std::vector<uint16_t>> ray_paths_;
};


class GeneralRayPathFilter : public AbstractRayPathFilter {
 public:
  void AddEntryFace(uint16_t face_number);
  void AddExitFace(uint16_t face_number);
  void AddHitNumber(int hit_num);
  void ClearFaces();
  void ClearHitNumbers();

 protected:
  bool FilterPath(const Crystal* crystal, RaySegment* last_r) const override;

 private:
  std::unordered_set<uint16_t> entry_faces_;
  std::unordered_set<uint16_t> exit_faces_;
  std::unordered_set<int> hit_nums_;
};


struct WavelengthInfo {
  int wavelength;
  float weight;
};


class ProjectContext {
 public:
  static ProjectContextPtrU CreateFromFile(const char* filename);
  static ProjectContextPtrU CreateDefault();

  size_t GetInitRayNum() const;
  void SetInitRayNum(size_t ray_num);

  int GetRayHitNum() const;
  void SetRayHitNum(int hit_num);

  std::string GetModelPath() const;
  void SetModelPath(const std::string& path);

  std::string GetDataDirectory() const;
  std::string GetDefaultImagePath() const;

  const Crystal* GetCrystal(int id) const;
  int32_t GetCrystalId(const Crystal* crystal) const;

#ifdef FOR_TEST
  void PrintCrystalInfo() const;
#endif

  static constexpr float kPropMinW = 1e-6;
  static constexpr float kScatMinW = 1e-3;
  static constexpr size_t kMinInitRayNum = 10000;
  static constexpr size_t kDefaultInitRayNum = 500000;
  static constexpr int kMinRayHitNum = 1;
  static constexpr int kMaxRayHitNum = 12;
  static constexpr int kDefaultRayHitNum = 8;

  SunContextPtr sun_ctx_;
  CameraContextPtr cam_ctx_;
  RenderContextPtr render_ctx_;
  std::vector<WavelengthInfo> wavelengths_;  // (wavelength, weight)
  std::vector<MultiScatterContext> multi_scatter_info_;

 private:
  ProjectContext();

  void ParseRaySettings(rapidjson::Document& d);
  void ParseDataSettings(const char* config_file_path, rapidjson::Document& d);
  void ParseCrystalSettings(rapidjson::Document& d);
  void ParseRayPathFilterSettings(rapidjson::Document& d);
  void ParseMultiScatterSettings(rapidjson::Document& d);

  void ParseOneCrystal(const rapidjson::Value& c);
  const CrystalContext* GetCrystalContext(int id) const;

  using FilterParser = std::function<RayPathFilterPtrU(const rapidjson::Value&, int)>;
  static std::unordered_map<std::string, FilterParser>& GetFilterParsers();
  void ParseOneFilter(const rapidjson::Value& c, int ci);
  static void ParseFilterBasic(const rapidjson::Value& c, int ci, const RayPathFilterPtrU& filter);
  static RayPathFilterPtrU ParseFilterNone(const rapidjson::Value& c, int ci);
  static RayPathFilterPtrU ParseFilterSpecific(const rapidjson::Value& c, int ci);
  static RayPathFilterPtrU ParseFilterGeneral(const rapidjson::Value& c, int ci);
  AbstractRayPathFilter* GetRayPathFilter(int id) const;

  void ParseOneScatter(const rapidjson::Value& c, int ci);

  size_t init_ray_num_;
  int ray_hit_num_;

  std::string model_path_;
  std::string data_path_;

  std::vector<CrystalContextPtrU> crystal_store_;
  std::unordered_map<int, RayPathFilterPtrU> filter_store_;
};


}  // namespace icehalo


#endif  // SRC_CONTEXT_CONTEXT_H_
