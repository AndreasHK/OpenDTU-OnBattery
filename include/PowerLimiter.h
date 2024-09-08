// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include "Configuration.h"
#include <espMqttClient.h>
#include <Arduino.h>
#include <Hoymiles.h>
#include <memory>
#include <functional>
#include <optional>
#include <TaskSchedulerDeclarations.h>
#include <frozen/string.h>

#define PL_UI_STATE_INACTIVE 0
#define PL_UI_STATE_CHARGING 1
#define PL_UI_STATE_USE_SOLAR_ONLY 2
#define PL_UI_STATE_USE_SOLAR_AND_BATTERY 3

class PowerLimiterClass {
public:
    enum class Status : unsigned {
        Initializing,
        DisabledByConfig,
        DisabledByMqtt,
        WaitingForValidTimestamp,
        PowerMeterPending,
        InverterInvalid,
        InverterChanged,
        InverterOffline,
        InverterCommandsDisabled,
        InverterLimitPending,
        InverterPowerCmdPending,
        InverterDevInfoPending,
        InverterStatsPending,
        CalculatedLimitBelowMinLimit,
        UnconditionalSolarPassthrough,
        NoVeDirect,
        NoEnergy,
        HuaweiPsu,
        Stable,
    };

    void init(Scheduler& scheduler);
    uint8_t getInverterUpdateTimeouts(size_t idx) const { return _settings[idx]._inverterUpdateTimeouts; }
    uint8_t getPowerLimiterState();
    int32_t getLastRequestedPowerLimit(size_t idx) { return _settings[idx]._lastRequestedPowerLimit; }
    bool getFullSolarPassThroughEnabled(size_t idx) const { return _settings[idx]._fullSolarPassThroughEnabled; }

    enum class Mode : unsigned {
        Normal = 0,
        Disabled = 1,
        UnconditionalFullSolarPassthrough = 2
    };

    void setMode(Mode m) { _mode = m; }
    Mode getMode() const { return _mode; }
    void calcNextInverterRestart();

private:

    struct DPLSettings{
        int32_t _lastRequestedPowerLimit = 0;
        bool _shutdownPending = false;
        std::optional<uint32_t> _oInverterStatsMillis = std::nullopt;
        std::optional<uint32_t> _oUpdateStartMillis = std::nullopt;
        std::optional<int32_t> _oTargetPowerLimitWatts = std::nullopt;
        std::optional<bool> _oTargetPowerState = std::nullopt;
        uint32_t _lastCalculation = 0;
        static constexpr uint32_t _calculationBackoffMsDefault = 128;
        uint32_t _calculationBackoffMs = _calculationBackoffMsDefault;
        std::shared_ptr<InverterAbstract> _inverter = nullptr;
        bool _batteryDischargeEnabled = false;
        bool _nighttimeDischarging = false;
        uint32_t _nextInverterRestart = 0; // Values: 0->not calculated / 1->no restart configured / >1->time of next inverter restart in millis()
        uint32_t _nextCalculateCheck = 5000; // time in millis for next NTP check to calulate restart
        bool _fullSolarPassThroughEnabled = false;
        uint8_t _inverterUpdateTimeouts = 0;
        float _loadFactor = 1.0f;
    };

    void loop();
    void loopAll();
    void loopOne(DPLSettings* _currentSettings, std::shared_ptr<InverterAbstract> currentInverter, int16_t maxPower);

    Task _loopTask;



    std::array<DPLSettings,2> _settings;
    DPLSettings* _currentSettings;

    uint32_t _lastLoop = 0;
    bool _verboseLogging = true;
    Mode _mode = Mode::Normal;
    Status _lastStatus = Status::Initializing;
    uint32_t _lastStatusPrinted = 0;


    frozen::string const& getStatusText(Status status);
    void announceStatus(Status status);
    bool shutdown(Status status);
    bool shutdown() { return shutdown(_lastStatus); }
    // float getBatteryVoltage(bool log = false);
    int32_t inverterPowerDcToAc(std::shared_ptr<InverterAbstract> inverter, int32_t dcPower);
    void unconditionalSolarPassthrough(std::shared_ptr<InverterAbstract> inverter);
    // bool canUseDirectSolarPower();
    bool calcPowerLimit(std::shared_ptr<InverterAbstract> inverter, int32_t solarPower, bool batteryPower, float loadFactor);
    bool updateInverter();
    bool setNewPowerLimit(std::shared_ptr<InverterAbstract> inverter, int32_t newPowerLimit);
    int32_t getSolarPower();
    float getLoadCorrectedVoltage();
    // bool testThreshold(float socThreshold, float voltThreshold,
    //         std::function<bool(float, float)> compare);
    // bool isStartThresholdReached();
    // bool isStopThresholdReached();
    // bool isBelowStopThreshold();
    bool useFullSolarPassthrough();
};

extern PowerLimiterClass PowerLimiter;
