from BlockDevice import BlockDevice
import os.path
import os

from ImageFile import ImageFile

class RawBlockDevice(BlockDevice):
  def __init__(self, raw_file, read_only=False, block_bytes=512, fobj=None):
    self.img_file = ImageFile(raw_file, read_only, block_bytes, fobj)

  def create(self, num_blocks):
    self.img_file.create(num_blocks)
    self.open()
    self.num_blocks = num_blocks

  def open(self):
    self.img_file.open()
    # calc block longs
    self.block_bytes = self.img_file.block_bytes
    self.block_longs = self.block_bytes / 4
    self.num_blocks = self.img_file.num_blocks

  def flush(self):
    self.img_file.flush()

  def close(self):
    self.img_file.close()

  def read_block(self, blk_num):
    return self.img_file.read_blk(blk_num)

  def write_block(self, blk_num, data):
    self.img_file.write_blk(blk_num, data)
