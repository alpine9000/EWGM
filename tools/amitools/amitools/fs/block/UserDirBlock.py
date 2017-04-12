import time
from Block import Block
from ..ProtectFlags import ProtectFlags

class UserDirBlock(Block):
  def __init__(self, blkdev, blk_num):
    Block.__init__(self, blkdev, blk_num, is_type=Block.T_SHORT, is_sub_type=Block.ST_USERDIR)
  
  def set(self, data):
    self._set_data(data)
    self._read()
  
  def read(self):
    self._read_data()
    self._read()
  
  def _read(self):
    Block.read(self)
    if not self.valid:
      return False

    # UserDir fields
    self.own_key = self._get_long(1)
    self.protect = self._get_long(-48)
    self.comment = self._get_bstr(-46, 79)
    self.mod_ts = self._get_timestamp(-23)
    self.name = self._get_bstr(-20, 30)
    self.hash_chain = self._get_long(-4)
    self.parent = self._get_long(-3)
    self.extension = self._get_long(-2)

    # hash table of entries
    self.hash_table = []
    self.hash_size = self.blkdev.block_longs - 56
    for i in xrange(self.hash_size):
      self.hash_table.append(self._get_long(6+i))
    
    self.valid = (self.own_key == self.blk_num)
    return self.valid

  def create(self, parent, name, protect=0, comment=None, mod_ts=None, hash_chain=0, extension=0):
    Block.create(self)
    self.own_key = self.blk_num
    self.protect = protect
    if comment == None:
      self.comment = ''
    else:
      self.comment = comment
    # timestamps
    self.mod_ts = mod_ts
    self.name = name
    self.hash_chain = hash_chain
    self.parent = parent
    self.extension = extension
    # empty hash table
    self.hash_table = []
    self.hash_size = self.blkdev.block_longs - 56
    for i in xrange(self.hash_size):
      self.hash_table.append(0)
    self.valid = True
    return True
    
  def write(self):
    Block._create_data(self)
    self._put_long(1, self.own_key)
    self._put_long(-48, self.protect)
    self._put_bstr(-46, 79, self.comment)
    self._put_timestamp(-23, self.mod_ts)
    self._put_bstr(-20, 30, self.name)
    self._put_long(-4, self.hash_chain)
    self._put_long(-3, self.parent)
    self._put_long(-2, self.extension)
    # hash table
    for i in xrange(self.hash_size):
      self._put_long(6+i, self.hash_table[i])
    Block.write(self)
  
  def dump(self):
    Block.dump(self,"UserDir")
    print " own_key:    %d" % (self.own_key)
    pf = ProtectFlags(self.protect)
    print " protect:    0x%x 0b%s %s" % (self.protect, pf.bin_str(), pf)
    print " comment:    '%s'" % self.comment
    print " mod_ts:     %s" % self.mod_ts
    print " name:       '%s'" % self.name
    print " hash_chain: %d" % self.hash_chain
    print " parent:     %d" % self.parent
    print " extension:  %s" % self.extension

