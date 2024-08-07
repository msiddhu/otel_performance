//
// Created by msiddhu on 7/16/24.
//

#ifndef OTEL_PERFORMANCE_TRACER_COMMON_H
#define OTEL_PERFORMANCE_TRACER_COMMON_H

// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "opentelemetry/sdk/trace/batch_span_processor_factory.h"
#include "opentelemetry/sdk/trace/exporter.h"
#include "opentelemetry/sdk/trace/processor.h"
#include "opentelemetry/sdk/trace/tracer_context.h"
#include "opentelemetry/sdk/trace/tracer_context_factory.h"
#include "opentelemetry/sdk/trace/tracer_provider_factory.h"
#include "opentelemetry/trace/provider.h"

#include "opentelemetry/exporters/otlp/otlp_file_exporter.h"
#include "opentelemetry/exporters/otlp/otlp_http_exporter.h"
#include "opentelemetry/exporters/otlp/otlp_http_exporter_options.h"


#include <cstring>
#include <iostream>
#include <vector>
#include <opentelemetry/sdk/trace/exporter.h>
#include "opentelemetry/ext/http/client/http_client.h"
#include "opentelemetry/nostd/shared_ptr.h"
#include <opentelemetry/exporters/otlp/otlp_http_exporter.h>
#include "opentelemetry/exporters/otlp/otlp_http_exporter_options.h"

namespace sdktrace = opentelemetry::sdk::trace;
namespace otlp = opentelemetry::exporter::otlp;

namespace
{


    void InitTracer()
    {

        opentelemetry::exporter::otlp::OtlpHttpExporterOptions http_exporter_options;
        http_exporter_options.url="http://localhost:4318/v1/traces";

        auto http_exporter = std::unique_ptr<sdktrace::SpanExporter>(
                new opentelemetry::exporter::otlp::OtlpHttpExporter(http_exporter_options));



        opentelemetry::sdk::trace::BatchSpanProcessorOptions batchSpanProcessorOptions;
        batchSpanProcessorOptions.max_queue_size=10000;

        auto processor =
                opentelemetry::sdk::trace::BatchSpanProcessorFactory::Create(std::move(http_exporter),batchSpanProcessorOptions);
        std::vector<std::unique_ptr<opentelemetry::sdk::trace::SpanProcessor>> processors;
        processors.push_back(std::move(processor));
        // Default is an always-on sampler.
        std::unique_ptr<opentelemetry::sdk::trace::TracerContext> context =
                opentelemetry::sdk::trace::TracerContextFactory::Create(std::move(processors));
        std::shared_ptr<opentelemetry::trace::TracerProvider> provider =
                opentelemetry::sdk::trace::TracerProviderFactory::Create(std::move(context));
        // Set the global trace provider
        opentelemetry::trace::Provider::SetTracerProvider(provider);
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
