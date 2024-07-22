#include <benchmark/benchmark.h>
#include <opentelemetry/exporters/otlp/otlp_http_exporter.h>
#include <opentelemetry/sdk/trace/simple_processor.h>
#include <opentelemetry/sdk/trace/tracer_provider.h>
#include <opentelemetry/trace/provider.h>
#include <opentelemetry/exporters/otlp/otlp_recordable.h>
#include <memory>
#include "tracer_common.h"

namespace trace = opentelemetry::trace;
namespace sdktrace = opentelemetry::sdk::trace;
namespace otlp = opentelemetry::exporter::otlp;
namespace nostd = opentelemetry::nostd;

using namespace std;


static void BM_SpanCreation(benchmark::State &state)
{
    InitTracer();
    auto tracer = get_tracer("normal") ;
    for (auto _ : state)
    {
        auto span = tracer->StartSpan("test_span");
        state.PauseTiming();
        span->End();
        state.ResumeTiming();
    }
}


static void BM_SpanEnd(benchmark::State &state)
{
    InitTracer();
    auto tracer = get_tracer("normal");

    for (auto _ : state)
    {
        state.PauseTiming();
        auto span = tracer->StartSpan("test_span");
        state.ResumeTiming();
        span->End();
    }
}

static void BM_SetAttribute(benchmark::State& state) {
    opentelemetry::exporter::otlp::OtlpRecordable recordable;
    for (auto _ : state) {
        recordable.SetAttribute("key", "value");
    }
}

static void BM_SpanCreationAndEnd(benchmark::State &state)
{
    InitTracer();
    auto tracer = get_tracer("normal") ;
    for (auto _ : state)
    {
        auto span = tracer->StartSpan("test_span");
        for(int i =0;i<10;i++){
            span->AddEvent("event"+std::to_string(i));
            span->SetAttribute("key"+std::to_string(i),"value"+std::to_string(i));
        }
        span->End();
    }
}

BENCHMARK(BM_SetAttribute);
BENCHMARK(BM_SpanCreation);
BENCHMARK(BM_SpanEnd);
BENCHMARK(BM_SpanCreationAndEnd);

BENCHMARK_MAIN();
