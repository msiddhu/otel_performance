#include <benchmark/benchmark.h>

#include <opentelemetry/sdk/trace/simple_processor.h>
#include <opentelemetry/sdk/trace/tracer_provider.h>
#include <opentelemetry/trace/provider.h>
#include <opentelemetry/exporters/otlp/otlp_recordable.h>
#include <memory>
#include <opentelemetry/common/timestamp.h>
#include "tracer_common.h"
#include <opentelemetry/sdk/common/global_log_handler.h>

namespace trace = opentelemetry::trace;
namespace sdktrace = opentelemetry::sdk::trace;
namespace otlp = opentelemetry::exporter::otlp;
namespace nostd = opentelemetry::nostd;
namespace common = opentelemetry::common;

using namespace std;

static void BM_AddEvent(benchmark::State &state) {

    InitTracer();
    auto tracer = get_tracer("normal");
    auto span = tracer->StartSpan("test_span");
    for (auto _ : state) {
        span->AddEvent("hello_event");
    }
    span->End();
}

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



static int end_count = 0;

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
        end_count++; // Increment the counter each time span->End() is called
    }
}
//
//static void BM_SetAttribute(benchmark::State& state) {
//    opentelemetry::exporter::otlp::OtlpRecordable recordable;
//    cout<<"recordable size" << recordable.span().ByteSizeLong()<< endl;
//
//    for (auto _ : state) {
//        recordable.SetAttribute("key", "value");
//    }
//    cout<<"recordable size" << recordable.span().ByteSizeLong()<< endl;
//}

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
//
//BENCHMARK(BM_AddEvent);
////BENCHMARK(BM_SetAttribute);
//BENCHMARK(BM_SpanCreation);
//BENCHMARK(BM_SpanEnd);

//int main(int argc, char** argv) {
//    ::benchmark::Initialize(&argc, argv);
//    ::benchmark::RunSpecifiedBenchmarks();
//    ::benchmark::Shutdown();
//    std::cout << "Span end called " << end_count << " times" << std::endl;
//
//
//}

void spanexample()
{
    int rand1=rand();
    auto tracer = get_tracer("normal"+std::to_string(rand1)) ;
    for(int i=0;i<100000;i++) {
    auto span = tracer->StartSpan("test_span");
    span->End();
    }
}

void spawnThreads(int n)
{
    std::vector<thread> threads(n);

    // spawn n threads:
    for (int i = 0; i < n; i++) {
        threads[i] = thread(spanexample);
    }

    for (auto& th : threads) {
        th.join();
    }
}


int main(){
    InitTracer();
    spanexample();
//    spawnThreads(1);
}
