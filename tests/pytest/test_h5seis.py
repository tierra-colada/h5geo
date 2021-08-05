import numpy as np
from h5gtpy import h5gt
from h5geopy import h5geo
import os.path
import unittest
import pathlib

trig = False

class test_h5seis(unittest.TestCase):
    def setUp(self):
        self.FILE_NAME = 'tmp/seis.h5'
        self.SEIS_NAME1 = 'path1/to/seis'
        self.SEIS_NAME2 = 'path2/to/seis'

        global trig
        if trig:
            file = h5gt.File(self.FILE_NAME, h5gt.OpenFlag(h5gt.OpenOrCreate))
            self.seisContainer = h5geo.createSeisContainer(file, h5geo.CreationType.OPEN_OR_CREATE)
        else:
            pathlib.Path('tmp').mkdir(exist_ok=True)
            file = h5gt.File(self.FILE_NAME, h5gt.OpenFlag(h5gt.OpenOrCreate | h5gt.Overwrite))
            self.seisContainer = h5geo.createSeisContainer(file, h5geo.CreationType.CREATE_OR_OVERWRITE)

        trig = True

        self.p.domain = h5geo.Domain.OWT
        self.p.spatialUnits = 'mm'
        self.p.temporalUnits = 'msec'
        self.p.dataUnits = 'kg'
        self.p.dataType = h5geo.SeisDataType.PRESTACK
        self.p.surveyType = h5geo.SurveyType.TWO_D
        self.p.nTrc = 30
        self.p.nSamp = 10
        self.p.srd = 20

    def tearDown(self):
        h5File = self.seisContainer.getH5File()

    seisContainer = None
    p = h5geo.SeisParam()
    FILE_NAME = None
    SEIS_NAME1 = None
    SEIS_NAME2 = None

    def test_createContainer(self):
        self.assertTrue(os.path.isfile(self.FILE_NAME))

    def test_createSeisWithDifferentCreateFlags(self):
        seis = self.seisContainer.createSeis(self.SEIS_NAME1, self.p, h5geo.CreationType.OPEN_OR_CREATE)
        self.assertFalse(seis is None)

        seis = self.seisContainer.createSeis(self.SEIS_NAME1, self.p, h5geo.CreationType.CREATE_OR_OVERWRITE)
        self.assertFalse(seis is None)

        seis = self.seisContainer.createSeis(self.SEIS_NAME1, self.p, h5geo.CreationType.CREATE_UNDER_NEW_NAME)
        self.assertFalse(seis is None)

        seis = self.seisContainer.createSeis(self.SEIS_NAME1, self.p, h5geo.CreationType.OPEN_OR_CREATE)
        self.assertFalse(seis is None)

    def test_createAndGetSeis(self):
        seis = self.seisContainer.createSeis(self.SEIS_NAME1, self.p, h5geo.CreationType.CREATE_OR_OVERWRITE)
        self.seisContainer.getSeis(self.SEIS_NAME1)
        self.assertFalse(seis is None)

    def test_createAndGetSeisFromGroup(self):
        group = self.seisContainer.getH5File().createGroup(self.SEIS_NAME2)
        self.seisContainer.createSeis(group, self.p, h5geo.CreationType.CREATE_OR_OVERWRITE)
        seis = self.seisContainer.getSeis(self.SEIS_NAME2)
        self.assertFalse(seis is None)

    def test_writeAndGetTextHeader(self):
        seis = self.seisContainer.createSeis(self.SEIS_NAME1, self.p, h5geo.CreationType.OPEN_OR_CREATE)
        self.assertFalse(seis is None)

        txtHdr = []
        for x in range(0, 40):
            txtHdr.append('Bart Simpson')

        self.assertTrue(seis.writeTextHeader(txtHdr))

        txtHdr_out = seis.getTextHeader()
        self.assertListEqual(txtHdr, txtHdr_out)

    def test_writeAndGetBinHeader(self):
        seis = self.seisContainer.createSeis(self.SEIS_NAME1, self.p, h5geo.CreationType.CREATE_OR_OVERWRITE)
        self.assertFalse(seis is None)

        binHdr = np.arange(30)
        self.assertTrue(seis.writeBinHeader(binHdr))

        binHdr_out = seis.getBinHeader()
        self.assertTrue(np.allclose(binHdr, binHdr_out))

        self.assertTrue(seis.writeBinHeader('SAMP_RATE', 2000))
        self.assertTrue(seis.getBinHeader('SAMP_RATE') == 2000)

    def test_writeAndGetTrace(self):
        seis = self.seisContainer.createSeis(self.SEIS_NAME1, self.p, h5geo.CreationType.CREATE_OR_OVERWRITE)
        self.assertFalse(seis is None)

        traces = np.random.rand(seis.getNSamp(), seis.getNTrc())
        self.assertTrue(seis.writeTrace(traces, 0, 0))     # write all traces at once

        traces_out = seis.getTrace(0, seis.getNTrc(), 0, seis.getNSamp())
        self.assertTrue(np.allclose(traces, traces_out))

        traces_out = seis.getTrace(3, 10, 2, 5)     # from 3 trc, 10 ntrc, from 2 sample, 5 nsamples
        self.assertTrue(np.allclose(traces[2:7, 3:13], traces_out))

        self.assertTrue(seis.writeTrace(traces[:, 10], 20, 0))  # write only one trace
        traces_out = seis.getTrace(20, 1, 0, seis.getNSamp())
        self.assertTrue(np.allclose(traces[:, 10], traces_out.flatten()))   # `flatten` makes 1D array from ND

    def test_writeAndGetTraceHeader(self):
        seis = self.seisContainer.createSeis(self.SEIS_NAME1, self.p, h5geo.CreationType.CREATE_OR_OVERWRITE)
        self.assertFalse(seis is None)

        trcHdr = np.random.rand(seis.getNTrc(), seis.getNTrcHdr())
        self.assertTrue(seis.writeTraceHeader(trcHdr, 0, 0))     # write all trace header at once

        trcHdr_out = seis.getTraceHeader(0, seis.getNTrc())
        self.assertTrue(np.allclose(trcHdr, trcHdr_out))

        self.assertTrue(seis.writeTraceHeader('CDP', trcHdr[:, 40]))
        trcHdr_out = seis.getTraceHeader('CDP')
        self.assertTrue(np.allclose(trcHdr[:, 40], trcHdr_out))

    def test_writeAndGetSortedData(self):
        seis = self.seisContainer.createSeis(self.SEIS_NAME1, self.p, h5geo.CreationType.CREATE_OR_OVERWRITE)
        self.assertFalse(seis is None)

        traces = np.random.rand(seis.getNSamp(), seis.getNTrc())
        self.assertTrue(seis.writeTrace(traces, 0, 0))     # write all traces at once

        trcHdr = np.random.randint(0, 5, [seis.getNTrc(), seis.getNTrcHdr()])
        self.assertTrue(seis.writeTraceHeader(trcHdr, 0, 0))     # write all traces headers at once

        self.assertTrue(seis.addPKeySort('FFID'))
        self.assertTrue(seis.addPKeySort('CDP'))
        self.assertTrue(seis.addPKeySort('DSREG'))

        trc_sorted, hdr_sorted, trc_ind = seis.getSortedData(
            ['FFID', 'CDP', 'DSREG'],
            [2, 1, 4],
            [4, 5, 5])

        print('hdr_sorted:')
        print(hdr_sorted)

        trcHdr_out = seis.getTraceHeader(0, seis.getNTrc())
        self.assertTrue(np.allclose(trcHdr, trcHdr_out))

        self.assertTrue(seis.writeTraceHeader('CDP', trcHdr[:, 40]))
        trcHdr_out = seis.getTraceHeader('CDP')
        self.assertTrue(np.allclose(trcHdr[:, 40], trcHdr_out))

if __name__ == '__main__':
    unittest.main()