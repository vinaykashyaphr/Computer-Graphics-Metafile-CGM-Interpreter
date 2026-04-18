# include "dispatchers/dispatcher.hpp"
# include "schemes/cgm_classes.hpp"
# include "dispatchers/metafile_desc.hpp"
# include "utils/generic.hpp"
# include "schemes/state.hpp"
# include "dispatchers/delimiter.hpp"




Dispatcher::Dispatcher(const std::vector<CgmElement>& elements):
    _elements(elements),
    _state(State()),
    _logger("_logs/dispatcher.log")
{}



void Dispatcher::dispatch() {

    std::ofstream log(_logger.data());

    for (const auto& el : _elements) {

        auto cgm_class = static_cast<CgmClass>(el.elem_class);

        switch (cgm_class) {

            case CgmClass::DELIMITER: {

                _Delimiter delimiter(_state, el, log);
                delimiter.dispatch();

                break;
            }

            case CgmClass::METAFILE_DESCRIPTOR: {

                _MetafileDescriptor metafile_descriptor(_state, el, log);
                metafile_descriptor.dispatch();
                break;
            }

            default:
                generic_utils::log_unknown(el, log);
                break;

        }

    }

}

