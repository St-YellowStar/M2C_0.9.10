
/// This file contains definitions of common data structures used in traffic manager.

#pragma once

#include <chrono>
#include <deque>
#include <vector>

#include "carla/client/Actor.h"
#include "carla/geom/Location.h"
#include "carla/geom/Rotation.h"
#include "carla/geom/Vector3D.h"
#include "carla/rpc/ActorId.h"
#include "carla/rpc/Command.h"
#include "carla/rpc/TrafficLightState.h"

#include "carla/trafficmanager/SimpleWaypoint.h"

namespace carla {
namespace traffic_manager {

namespace chr = std::chrono;
namespace cc = carla::client;
namespace cg = carla::geom;

using ActorId = carla::ActorId;
using ActorPtr = carla::SharedPtr<cc::Actor>;
using JunctionID = carla::road::JuncId;
using SimpleWaypointPtr = std::shared_ptr<SimpleWaypoint>;
using Buffer = std::deque<SimpleWaypointPtr>;
using BufferMap = std::unordered_map<carla::ActorId, Buffer>;
using TimeInstance = chr::time_point<chr::system_clock, chr::nanoseconds>;
using TLS = carla::rpc::TrafficLightState;


/////MTS Extension

struct MTS_Leader{
  boost::optional<ActorId> MainLeader;
  boost::optional<ActorId> PotentialLeader;
};

struct MTS_Neighbor
{
  boost::optional<ActorId> LeftVehicle;
  boost::optional<ActorId> RightVehicle;
  boost::optional<ActorId> LeftFrontVehicle;
  boost::optional<ActorId> RightFrontVehicle;
  boost::optional<ActorId> LeftRearVehicle;
  boost::optional<ActorId> RightRearVehicle;
};

struct MTS_Region
{
  boost::optional<ActorId> leftBorderVehicle;
  boost::optional<ActorId> rightBorderVehicle;
  std::vector<boost::optional<ActorId>> frontVehicles;
  std::vector<boost::optional<ActorId>> rearVehicles;
  
  cg::Location leftBorder;
  cg::Location rightBorder;
  cg::Location location;
  //cg::Rotation rotation;

  float gap;
  float width;
  float maxPassingSpeed;
  float safety;
  float preference;
};

struct MTS_SituationData 
{
  std::vector<MTS_Region>	CandidateRegions;
  MTS_Region CurrentRegion;
  bool SpaceOriented = false;
  float safety;
  float desiredOffset;
};




struct LocalizationData {
  SimpleWaypointPtr junction_end_point;
  SimpleWaypointPtr safe_point;
  bool is_at_junction_entrance;

  /////MTS Extension

  MTS_Leader leader;
  MTS_Neighbor neighbor;
  MTS_SituationData situation;
};
using LocalizationFrame = std::vector<LocalizationData>;

struct CollisionHazardData {
  float available_distance_margin;
  ActorId hazard_actor_id;
  bool hazard;
};
using CollisionFrame = std::vector<CollisionHazardData>;

using ControlFrame = std::vector<carla::rpc::Command>;

using TLFrame = std::vector<bool>;

/// Structure to hold the actuation signals.
struct ActuationSignal {
  float throttle;
  float brake;
  float steer;
};

/// Structure to hold the controller state.
struct StateEntry {
  cc::Timestamp time_instance;
  float deviation;
  float velocity;
  float deviation_integral;
  float velocity_integral;
};


} // namespace traffic_manager
} // namespace carla
