package cfs.data.dao;

import cfs.data.entity.Role;
import java.util.List;
import org.springframework.data.repository.CrudRepository;

/**
 * Role data access object. The implementation of the interface is
 * generated automatically during the runtime.
 */

public interface RoleDao extends CrudRepository<Role, Long> {
  public List<Role> findByEmployeeId(Long id);
}
