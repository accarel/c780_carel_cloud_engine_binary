#!/usr/bin/env python

from __future__ import division, print_function, unicode_literals

import ttfw_idf


@ttfw_idf.idf_GME_Binary(env_tag='Example_GENERIC', target=['esp32', 'esp32s2', 'esp32c3'], ci_target=['esp32'])
def GME_Binary(env, extra_data):
    app_name = 'GME_Binary'
    dut = env.get_dut(app_name, '/user/esp/GME_Binary')
    dut.start_app()
    res = dut.expect(ttfw_idf.MINIMUM_FREE_HEAP_SIZE_RE)
    if not res:
        raise ValueError('Maximum heap size info not found')
    ttfw_idf.print_heap_size(app_name, dut.app.config_name, dut.TARGET, res[0])


if __name__ == '__main__':
    app_main()
