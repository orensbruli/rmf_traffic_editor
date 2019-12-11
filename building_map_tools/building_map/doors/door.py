from xml.etree.ElementTree import Element, SubElement


class Door:
    def __init__(self, door_edge):
        self.name = door_edge.params['name'].value
        self.length = door_edge.length
        self.cx = door_edge.x
        self.cy = door_edge.y
        self.yaw = door_edge.yaw
        self.height = 2.5  # parameterize someday?
        self.thickness = 0.03  # parameterize someday?
        print(f'Door({self.name})')

        self.model_ele = Element('model')
        self.model_ele.set('name', self.name)
        pose_ele = SubElement(self.model_ele, 'pose')
        pose_ele.text = f'{self.cx} {self.cy} 0 0 0 {self.yaw}'

    def generate_sliding_section(self, name, width, x_offset, bounds):
        link_ele = SubElement(self.model_ele, 'link')
        link_ele.set('name', name)
        pose_ele = SubElement(link_ele, 'pose')
        pose_ele.text = f'{x_offset} 0 {self.height/2+0.01} 0 0 0'

        visual_ele = SubElement(link_ele, 'visual')
        visual_ele.set('name', name)
        visual_ele.append(self.material())
        visual_geometry_ele = SubElement(visual_ele, 'geometry')
        visual_geometry_ele.append(
            self.box(width, self.thickness, self.height))

        collision_ele = SubElement(link_ele, 'collision')
        collision_ele.set('name', name)
        collision_ele.append(self.collide_bitmask())
        collision_geometry_ele = SubElement(collision_ele, 'geometry')
        collision_geometry_ele.append(
            self.box(width, self.thickness, self.height))

        # now, the joint for this link
        joint_ele = SubElement(self.model_ele, 'joint')
        joint_ele.set('name', f'{name}_joint')
        joint_ele.set('type', 'prismatic')

        parent_ele = SubElement(joint_ele, 'parent')
        parent_ele.text = 'world'

        child_ele = SubElement(joint_ele, 'child')
        child_ele.text = name

        axis_ele = SubElement(joint_ele, 'axis')
        axis_ele.text = '1 0 0'

        limit_ele = SubElement(axis_ele, 'limit')
        lower_ele = SubElement(limit_ele, 'lower')
        lower_ele.text = str(bounds[0])
        upper_ele = SubElement(limit_ele, 'upper')
        upper_ele.text = str(bounds[1])

    def collide_bitmask(self):
        surface_ele = Element('surface')
        contact_ele = SubElement(surface_ele, 'contact')
        collide_bitmask_ele = SubElement(contact_ele, 'collide_bitmask')
        collide_bitmask_ele.text = '0x02'
        return surface_ele

    def box(self, x, y, z):
        box_ele = Element('box')
        size_ele = SubElement(box_ele, 'size')
        size_ele.text = f'{x} {y} {z}'
        return box_ele

    def material(self):
        material_ele = Element('material')
        # blue-green glass as a default, so it's easy to see
        ambient_ele = SubElement(material_ele, 'ambient')
        ambient_ele.text = '{} {} {} {}'.format(128, 192, 210, 0.6)
        diffuse_ele = SubElement(material_ele, 'diffuse')
        diffuse_ele.text = '{} {} {} {}'.format(128, 192, 210, 0.6)
        return material_ele
