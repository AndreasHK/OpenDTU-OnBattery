<template>
    <div class="row row-cols-1 row-cols-md-3 g-3">
        <div class="col">
            <CardElement
                centerContent
                textVariant="text-bg-success"
                :text="$t('invertertotalinfo.InverterTotalYieldTotal')"
            >
                <h2>
                    {{
                        $n(totalData.YieldTotal.v, 'decimal', {
                            minimumFractionDigits: totalData.YieldTotal.d,
                            maximumFractionDigits: totalData.YieldTotal.d,
                        })
                    }}
                    <small class="text-muted">{{ totalData.YieldTotal.u }}</small>
                </h2>
            </CardElement>
        </div>
        <div class="col">
            <CardElement
                centerContent
                textVariant="text-bg-success"
                :text="$t('invertertotalinfo.InverterTotalYieldDay')"
            >
                <h2>
                    {{
                        $n(totalData.YieldDay.v, 'decimal', {
                            minimumFractionDigits: totalData.YieldDay.d,
                            maximumFractionDigits: totalData.YieldDay.d,
                        })
                    }}
                    <small class="text-muted">{{ totalData.YieldDay.u }}</small>
                </h2>
            </CardElement>
        </div>
        <div class="col">
            <CardElement centerContent textVariant="text-bg-success" :text="$t('invertertotalinfo.InverterTotalPower')">
                <h2>
                    {{
                        $n(totalData.Power.v, 'decimal', {
                            minimumFractionDigits: totalData.Power.d,
                            maximumFractionDigits: totalData.Power.d,
                        })
                    }}
                    <small class="text-muted">{{ totalData.Power.u }}</small>
                </h2>
            </CardElement>
        </div>
        <div class="col" v-if="powerMeterData.enabled">
            <CardElement centerContent textVariant="text-bg-success" :text="$t('invertertotalinfo.HomePower')">
                <h2>
                    {{
                        $n(powerMeterData.Power.v, 'decimal', {
                            minimumFractionDigits: powerMeterData.Power.d,
                            maximumFractionDigits: powerMeterData.Power.d,
                        })
                    }}
                    <small class="text-muted">{{ powerMeterData.Power.u }}</small>
                </h2>
            </CardElement>
        </div>
    </div>
</template>

<script lang="ts">
import type { Total, PowerMeter } from '@/types/LiveDataStatus';
import CardElement from './CardElement.vue';
import { defineComponent, type PropType } from 'vue';

export default defineComponent({
    components: {
        CardElement,
    },
    props: {
        totalData: { type: Object as PropType<Total>, required: true },
        powerMeterData: { type: Object as PropType<PowerMeter>, required: true },
    },
});
</script>
