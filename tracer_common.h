//
// Created by msiddhu on 7/16/24.
//

#ifndef OTEL_PERFORMANCE_TRACER_COMMON_H
#define OTEL_PERFORMANCE_TRACER_COMMON_H

// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "opentelemetry/exporters/ostream/span_exporter_factory.h"
#include "opentelemetry/sdk/trace/exporter.h"
#include "opentelemetry/sdk/trace/processor.h"
#include "opentelemetry/sdk/trace/simple_processor_factory.h"
#include "opentelemetry/sdk/trace/tracer_context.h"
#include "opentelemetry/sdk/trace/tracer_context_factory.h"
#include "opentelemetry/sdk/trace/tracer_provider_factory.h"
#include "opentelemetry/trace/provider.h"

#include "opentelemetry/context/propagation/global_propagator.h"
#include "opentelemetry/context/propagation/text_map_propagator.h"
#include "opentelemetry/trace/propagation/http_trace_context.h"

#include <cstring>
#include <iostream>
#include <vector>
#include <opentelemetry/sdk/trace/exporter.h>
#include "opentelemetry/ext/http/client/http_client.h"
#include "opentelemetry/nostd/shared_ptr.h"

namespace sdktrace = opentelemetry::sdk::trace;

namespace
{


    void InitTracer()
    {
        auto exporter = std::unique_ptr<sdktrace::SpanExporter>(
                                               new opentelemetry::exporter::otlp::OtlpHttpExporter());
        auto processor =
                opentelemetry::sdk::trace::SimpleSpanProcessorFactory::Create(std::move(exporter));
        std::vector<std::unique_ptr<opentelemetry::sdk::trace::SpanProcessor>> processors;
        processors.push_back(std::move(processor));
        // Default is an always-on sampler.
        std::unique_ptr<opentelemetry::sdk::trace::TracerContext> context =
                opentelemetry::sdk::trace::TracerContextFactory::Create(std::move(processors));
        std::shared_ptr<opentelemetry::trace::TracerProvider> provider =
                opentelemetry::sdk::trace::TracerProviderFactory::Create(std::move(context));
        // Set the global trace provider
        opentelemetry::trace::Provider::SetTracerProvider(provider);

        // set global propagator
        opentelemetry::context::propagation::GlobalTextMapPropagator::SetGlobalPropagator(
                opentelemetry::nostd::shared_ptr<opentelemetry::context::propagation::TextMapPropagator>(
                        new opentelemetry::trace::propagation::HttpTraceContext()));
    }

    void CleanupTracer()
    {
        std::shared_ptr<opentelemetry::trace::TracerProvider> none;
        opentelemetry::trace::Provider::SetTracerProvider(none);
    }

    opentelemetry::nostd::shared_ptr<opentelemetry::trace::Tracer> get_tracer(std::string tracer_name)
    {
        auto provider = opentelemetry::trace::Provider::GetTracerProvider();
        return provider->GetTracer(tracer_name);
    }

}  // namespace
#endif //OTEL_PERFORMANCE_TRACER_COMMON_H
